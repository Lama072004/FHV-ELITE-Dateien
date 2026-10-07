import 'package:flutter/material.dart';
import 'dart:math' as math;
import '../theme/app_theme.dart';

class WeightGauge extends StatelessWidget {
  final double? currentWeight;
  final double maxWeight;
  final bool isRecipeMode;
  final double? targetWeight;
  final double tolerance;

  const WeightGauge({
    required this.currentWeight,
    required this.maxWeight,
    this.isRecipeMode = false,
    this.targetWeight,
    this.tolerance = 5.0,
    super.key,
  });

  Color _getColor(double percentage) {
    if (percentage <= 0.5) {
      final t = percentage * 2;
      return Color.lerp(
        const Color(0xFF6A994E),
        const Color(0xFFFFD700),
        t,
      )!;
    } else {
      final t = (percentage - 0.5) * 2;
      return Color.lerp(
        const Color(0xFFFFD700),
        const Color(0xFFBC4749),
        t,
      )!;
    }
  }

  Color _getColorByDistance(double normalizedDist) {
    if (normalizedDist <= 0.5) {
      return Color.lerp(const Color(0xFF6A994E), const Color(0xFFFFD700), normalizedDist * 2)!;
    } else {
      return Color.lerp(const Color(0xFFFFD700), const Color(0xFFBC4749), (normalizedDist - 0.5) * 2)!;
    }
  }

  @override
  Widget build(BuildContext context) {
    double rawPercentage = 0.0; // may be <0 or >1
    Color gaugeColor = AppTheme.green;
    double tolerancePercentForPainter = 0.0; // percent of full semicircle (0..0.5)
    double minVisual = 0.0;
    double maxVisual = maxWeight;

    if (isRecipeMode && targetWeight != null) {
      // Visual range centers target at 50%: [0, 2*target]
      minVisual = 0.0;
      maxVisual = (targetWeight! * 2.0).clamp(1.0, double.infinity);
      if (currentWeight != null) {
        rawPercentage = (currentWeight! - minVisual) / (maxVisual - minVisual);
      } else {
        rawPercentage = 0.0;
      }
      // tolerance interpreted as percent (e.g. 5.0 => 5%)
      final tolFraction = (tolerance / 100.0).clamp(0.0, 1.0);
      tolerancePercentForPainter = (tolFraction / 2.0).clamp(0.0, 0.5);
      // color by distance from center (50%)
      final normalizedDist = ((rawPercentage - 0.5).abs() / 0.5).clamp(0.0, 1.0);
      gaugeColor = _getColorByDistance(normalizedDist);
    } else {
      // normal mode
      if (currentWeight != null && currentWeight! >= 0) {
        rawPercentage = (currentWeight! / maxWeight);
        rawPercentage = rawPercentage.clamp(0.0, 1.0);
        gaugeColor = _getColor(rawPercentage);
      } else {
        rawPercentage = 0.0;
      }
      tolerancePercentForPainter = (tolerance / maxWeight).clamp(0.0, 0.5);
    }

    return SizedBox(
      width: 260,
      height: 170,
      child: CustomPaint(
        painter: GaugePainter(
          rawPercentage: rawPercentage,
          gaugeColor: gaugeColor,
          isRecipeMode: isRecipeMode,
          tolerancePercent: tolerancePercentForPainter,
          targetWeight: targetWeight,
          minVisual: minVisual,
          maxVisual: maxVisual,
        ),
      ),
    );
  }
}

class GaugePainter extends CustomPainter {
  final double rawPercentage; // may be <0 or >1
  final Color gaugeColor;
  final bool isRecipeMode;
  final double tolerancePercent;
  final double? targetWeight;
  final double? minVisual;
  final double? maxVisual;

  GaugePainter({
    required this.rawPercentage,
    required this.gaugeColor,
    required this.isRecipeMode,
    this.tolerancePercent = 0.0,
    this.targetWeight,
    this.minVisual,
    this.maxVisual,
  });

  @override
  void paint(Canvas canvas, Size size) {
    final center = Offset(size.width / 2, size.height / 1.25);
    final radius = size.width * 0.45;

    // Background semicircle
    final backgroundPaint = Paint()
      ..color = const Color(0xFF444444)
      ..strokeWidth = 14
      ..strokeCap = StrokeCap.round
      ..style = PaintingStyle.stroke;
    canvas.drawArc(
      Rect.fromCenter(center: center, width: radius * 2, height: radius * 2),
      math.pi,
      math.pi,
      false,
      backgroundPaint,
    );

    // Active arc (clamped to 0..1 -> max 180deg)
    final visiblePct = rawPercentage.clamp(0.0, 1.0);
    final activePaint = Paint()
      ..shader = SweepGradient(
        startAngle: math.pi,
        endAngle: math.pi + math.pi * visiblePct,
        colors: [gaugeColor.withOpacity(0.95), gaugeColor],
      ).createShader(Rect.fromCircle(center: center, radius: radius))
      ..strokeWidth = 14
      ..strokeCap = StrokeCap.round
      ..style = PaintingStyle.stroke;

    canvas.drawArc(
      Rect.fromCenter(center: center, width: radius * 2, height: radius * 2),
      math.pi,
      math.pi * visiblePct,
      false,
      activePaint,
    );

    // Tick marks
    final markPaint = Paint()..color = const Color(0xFFAAAAAA)..strokeWidth = 2;
    for (int i = 0; i <= 10; i++) {
      final angle = math.pi + (math.pi * i / 10);
      final start = Offset(center.dx + radius * math.cos(angle), center.dy + radius * math.sin(angle));
      final end = Offset(center.dx + (radius - 10) * math.cos(angle), center.dy + (radius - 10) * math.sin(angle));
      canvas.drawLine(start, end, markPaint);
    }

    // Tolerance arc centered at 50% in recipe mode
    if (isRecipeMode && tolerancePercent > 0) {
      final tolPaint = Paint()
        ..color = const Color(0xFF6A994E).withOpacity(0.28)
        ..strokeWidth = 12
        ..style = PaintingStyle.stroke;
      final startPercent = 0.5 - tolerancePercent;
      final sweep = tolerancePercent * 2;
      canvas.drawArc(
        Rect.fromCenter(center: center, width: radius * 2, height: radius * 2),
        math.pi + (math.pi * startPercent),
        math.pi * sweep,
        false,
        tolPaint,
      );
    }

    // Target marker at center (50%) in recipe mode
    if (isRecipeMode && targetWeight != null) {
      final targetAngle = math.pi + math.pi * 0.5;
      final inner = radius - 22;
      final outer = radius + 6;
      final markerPaint = Paint()..color = AppTheme.cream..strokeWidth = 3..strokeCap = StrokeCap.round;
      final sx = center.dx + inner * math.cos(targetAngle);
      final sy = center.dy + inner * math.sin(targetAngle);
      final ex = center.dx + outer * math.cos(targetAngle);
      final ey = center.dy + outer * math.sin(targetAngle);
      canvas.drawLine(Offset(sx, sy), Offset(ex, ey), markerPaint);

      // Numeric label above the marker (always shows target weight)
      final label = '${targetWeight!.toStringAsFixed(targetWeight! % 1 == 0 ? 0 : 1)} g';
      final textPainter = TextPainter(
        text: TextSpan(
          text: label,
          style: TextStyle(color: AppTheme.cream, fontSize: 14, fontWeight: FontWeight.bold),
        ),
        textDirection: TextDirection.ltr,
      );
      textPainter.layout();
      final labelX = center.dx - textPainter.width / 2;
      final labelY = center.dy - radius - textPainter.height - 6; // place above semicircle top
      textPainter.paint(canvas, Offset(labelX, labelY));
    }

    // Current value pointer (clamped to visible arc for drawing)
    final clamped = rawPercentage.clamp(0.0, 1.0);
    final clampedAngle = math.pi + math.pi * clamped;
    final px = center.dx + (radius - 6) * math.cos(clampedAngle);
    final py = center.dy + (radius - 6) * math.sin(clampedAngle);
    final pointerPaint = Paint()..color = gaugeColor;
    canvas.drawCircle(Offset(px, py), 8, pointerPaint);
  }

  @override
  bool shouldRepaint(GaugePainter oldDelegate) {
    return oldDelegate.rawPercentage != rawPercentage ||
        oldDelegate.gaugeColor != gaugeColor ||
        oldDelegate.tolerancePercent != tolerancePercent ||
        oldDelegate.targetWeight != targetWeight;
  }
}
