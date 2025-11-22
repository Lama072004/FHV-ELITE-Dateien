import 'package:flutter/foundation.dart';

class WeightService {
  // Notifier für das aktuelle Gewicht in Gramm
  static final ValueNotifier<double?> currentWeight = ValueNotifier<double?>(null);

  static void setWeight(double? w) => currentWeight.value = w;
  static double? getWeight() => currentWeight.value;
}
