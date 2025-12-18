import 'package:flutter/material.dart';
import '../theme/app_theme.dart';
import '../models/recipe.dart';
import '../services/weight_service.dart';
import '../widgets/weight_gauge.dart';

/// Screen zum schrittweisen Nachkochen eines Rezepts mit Gewichtskontrolle
class RecipeFollowerScreen extends StatefulWidget {
  final Recipe recipe;

  const RecipeFollowerScreen({required this.recipe, super.key});

  @override
  State<RecipeFollowerScreen> createState() => _RecipeFollowerScreenState();
}

class _RecipeFollowerScreenState extends State<RecipeFollowerScreen> {
  late int servings;
  late List<Ingredient> scaledIngredients;
  late TextEditingController servingsController;
  int? selectedIngredientIndex;
  bool _completedShown = false;

  // Verfolge abgeschlossene Zutaten
  final Set<int> completedIndices = {};

  @override
  void initState() {
    super.initState();
    servings = widget.recipe.servings;
    servingsController = TextEditingController(text: servings.toString());
    servingsController.addListener(() {
      final v = int.tryParse(servingsController.text) ?? servings;
      if (v != servings && v > 0) {
        servings = v;
        _updateScaledIngredients();
      }
    });
    _updateScaledIngredients();
  }

  @override
  void dispose() {
    servingsController.dispose();
    super.dispose();
  }

  /// Skaliere Zutaten entsprechend der Portionen
  void _updateScaledIngredients() {
    final scale = servings / widget.recipe.servings;
    scaledIngredients = widget.recipe.ingredients
        .map((ing) => Ingredient(
              name: ing.name,
              amount: ing.amount * scale,
              unit: ing.unit,
            ))
        .toList();
    setState(() {});
  }

  /// Prüfe, ob aktuelles Gewicht in Toleranz der ausgewählten Zutat liegt
  Future<void> _onWeightChanged(double? weight, Ingredient? target) async {
    if (target == null) return;
    if (weight == null) {
      _completedShown = false;
      return;
    }
    final tol = 5.0; // Toleranz in Gramm
    final minW = target.amount - tol;
    final maxW = target.amount + tol;

    if (weight >= minW && weight <= maxW) {
      if (!_completedShown) {
        _completedShown = true;
        // Automatische Glückwunsch-Meldung entfernt; Nutzer muss manuell markieren
      }
    } else {
      _completedShown = false;
    }
  }

  /// Markiere Zutat als erledigt
  void _markIngredientDone(int index) {
    setState(() {
      completedIndices.add(index);
    });
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(
        content: Text('Zutat "${scaledIngredients[index].name}" markiert als erledigt'),
        backgroundColor: AppTheme.green,
      ),
    );
  }

  /// Prüfe, ob Einheit ohne Gewicht markiert werden darf (Stück, EL, TL, g, ml etc.)
  bool isMarkAllowedWithoutWeight(String u) {
    final norm = u.toLowerCase().replaceAll(RegExp(r'[^a-z0-9]'), '');
    const allowed = {
      'stk', 'stuck', 'stück', 'stücke', 'pcs', 'piece', 'tasse', 'cup',
      'el', 'esslffel', 'essloffel', 'el.', 'tl', 'teelffel', 'teelloefel', 'tl.',
      'g', 'gram', 'gramm', 'kg', 'ml', 'milliliter', 'l', 'liter'
    };
    return allowed.contains(norm);
  }

  @override
  Widget build(BuildContext context) {
    final selectedIng = selectedIngredientIndex != null &&
            selectedIngredientIndex! < scaledIngredients.length
        ? scaledIngredients[selectedIngredientIndex!]
        : null;

    return Scaffold(
      appBar: AppBar(title: const Text('Rezept folgen')),
      body: SingleChildScrollView(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            // Rezeptname
            Text(
              widget.recipe.name,
              style: Theme.of(context).textTheme.headlineSmall?.copyWith(
                    color: AppTheme.lightGreen,
                    fontWeight: FontWeight.bold,
                  ),
            ),
            const SizedBox(height: 24),

            // Gauge mit Live-Gewicht
            Center(
              child: ValueListenableBuilder<double?>(
                valueListenable: WeightService.currentWeight,
                builder: (context, weight, _) {
                  _onWeightChanged(weight, selectedIng);
                  return WeightGauge(
                    currentWeight: weight,
                    maxWeight: (selectedIng?.amount ?? 500),
                    isRecipeMode: true,
                    targetWeight: selectedIng?.amount,
                    tolerance: 5.0,
                  );
                },
              ),
            ),
            const SizedBox(height: 12),

            // Live-Gewichtsanzeige
            Center(
              child: ValueListenableBuilder<double?>(
                valueListenable: WeightService.currentWeight,
                builder: (context, weight, _) {
                  return Text(
                    weight == null ? '... g' : '${weight.toStringAsFixed(1)} g',
                    textAlign: TextAlign.center,
                    style: const TextStyle(
                      color: AppTheme.lightGreen,
                      fontSize: 20,
                      fontWeight: FontWeight.bold,
                    ),
                  );
                },
              ),
            ),
            const SizedBox(height: 20),

            // Portionen-Rechner
            Container(
              decoration: BoxDecoration(
                color: const Color(0xFF2A2A2A),
                borderRadius: BorderRadius.circular(12),
              ),
              padding: const EdgeInsets.all(16),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    'Portionen',
                    style: Theme.of(context).textTheme.titleSmall?.copyWith(
                          color: AppTheme.green,
                        ),
                  ),
                  const SizedBox(height: 12),
                  Row(
                    children: [
                      IconButton(
                        icon: const Icon(Icons.remove, color: AppTheme.red),
                        onPressed: servings > 1
                            ? () {
                                setState(() => servings--);
                                servingsController.text = servings.toString();
                                _updateScaledIngredients();
                              }
                            : null,
                      ),
                      Expanded(
                        child: TextField(
                          controller: servingsController,
                          textAlign: TextAlign.center,
                          keyboardType: TextInputType.number,
                          decoration: const InputDecoration(
                            border: OutlineInputBorder(),
                          ),
                        ),
                      ),
                      IconButton(
                        icon: const Icon(Icons.add, color: AppTheme.green),
                        onPressed: () {
                          setState(() => servings++);
                          servingsController.text = servings.toString();
                          _updateScaledIngredients();
                        },
                      ),
                    ],
                  ),
                ],
              ),
            ),
            const SizedBox(height: 24),

            // Zubereitungsschritte (falls vorhanden)
            if (widget.recipe.steps.isNotEmpty) ...[
              Text(
                'Zubereitungsschritte',
                style: Theme.of(context).textTheme.titleMedium?.copyWith(
                      color: AppTheme.lightGreen,
                      fontWeight: FontWeight.bold,
                    ),
              ),
              const SizedBox(height: 12),
              ...widget.recipe.steps.asMap().entries.map((e) {
                final idx = e.key;
                final step = e.value;
                return Padding(
                  padding: const EdgeInsets.only(bottom: 12),
                  child: Container(
                    decoration: BoxDecoration(
                      color: const Color(0xFF2A2A2A),
                      borderRadius: BorderRadius.circular(8),
                    ),
                    padding: const EdgeInsets.all(12),
                    child: Row(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        CircleAvatar(
                          backgroundColor: AppTheme.cream,
                          radius: 16,
                          child: Text(
                            '${idx + 1}',
                            style: const TextStyle(
                              color: AppTheme.darkGreen,
                              fontWeight: FontWeight.bold,
                            ),
                          ),
                        ),
                        const SizedBox(width: 12),
                        Expanded(
                          child: Text(
                            step,
                            style: const TextStyle(color: Colors.white),
                          ),
                        ),
                      ],
                    ),
                  ),
                );
              }),
              const SizedBox(height: 24),
            ],

            // Zutaten mit Markierungsfunktion
            Text(
              'Zutaten',
              style: Theme.of(context).textTheme.titleMedium?.copyWith(
                    color: AppTheme.lightGreen,
                    fontWeight: FontWeight.bold,
                  ),
            ),
            const SizedBox(height: 12),
            ...scaledIngredients.asMap().entries.map((e) {
              final idx = e.key;
              final ing = e.value;
              final isSelected = selectedIngredientIndex == idx;

              return ValueListenableBuilder<double?>(
                valueListenable: WeightService.currentWeight,
                builder: (context, weight, _) {
                  final tol = 5.0;
                  final withinTol = weight != null &&
                      weight >= (ing.amount - tol) &&
                      weight <= (ing.amount + tol);
                  final isDone = completedIndices.contains(idx);

                  // Erlaube sofortiges Markieren für bestimmte Einheiten
                  final allowWithoutWeight = isMarkAllowedWithoutWeight(ing.unit);

                  return GestureDetector(
                    onTap: () => setState(() => selectedIngredientIndex = idx),
                    child: Padding(
                      padding: const EdgeInsets.only(bottom: 12),
                      child: Container(
                        decoration: BoxDecoration(
                          color: const Color(0xFF2A2A2A),
                          borderRadius: BorderRadius.circular(8),
                          // Grüner Rahmen nur bei Auswahl
                          border: isSelected ? Border.all(color: AppTheme.lightGreen, width: 2) : null,
                        ),
                        padding: const EdgeInsets.all(12),
                        child: Row(
                          children: [
                            Expanded(
                              child: Column(
                                crossAxisAlignment: CrossAxisAlignment.start,
                                children: [
                                  Text(
                                    ing.name,
                                    style: TextStyle(
                                      fontWeight: FontWeight.bold,
                                      color: isDone ? Colors.grey : null,
                                    ),
                                  ),
                                  const SizedBox(height: 4),
                                  Text(
                                    '${ing.amount.toStringAsFixed(ing.amount % 1 == 0 ? 0 : 2)} ${ing.unit}',
                                    style: TextStyle(
                                      color: isDone ? Colors.grey : AppTheme.lightGreen,
                                      fontSize: 14,
                                      fontWeight: FontWeight.bold,
                                    ),
                                  ),
                                ],
                              ),
                            ),
                            // Trailing: Haken wenn erledigt, sonst Button zum Markieren
                            if (isDone)
                              const Icon(Icons.check_circle, color: AppTheme.lightGreen)
                            else if (isSelected)
                              IconButton(
                                icon: const Icon(Icons.check),
                                color: (withinTol || allowWithoutWeight) ? AppTheme.lightGreen : Colors.grey,
                                onPressed: () async {
                                  if (withinTol || allowWithoutWeight) {
                                    _markIngredientDone(idx);
                                  } else {
                                    // Bestätigungsdialog bei manueller Markierung außerhalb Toleranz
                                    final ok = await showDialog<bool>(
                                      context: context,
                                      builder: (context) => AlertDialog(
                                        backgroundColor: const Color(0xFF1E1E1E),
                                        title: const Text('Außerhalb der Toleranz'),
                                        content: Text(
                                            'Das aktuelle Gewicht (${weight?.toStringAsFixed(1) ?? '—'} g) liegt nicht innerhalb der Toleranz für ${ing.name} (${ing.amount.toStringAsFixed(1)} ${ing.unit}). Möchtest du trotzdem markieren?'),
                                        actions: [
                                          TextButton(
                                            onPressed: () => Navigator.of(context).pop(false),
                                            child: const Text('Abbrechen'),
                                          ),
                                          ElevatedButton(
                                            onPressed: () => Navigator.of(context).pop(true),
                                            child: const Text('Trotzdem markieren'),
                                          ),
                                        ],
                                      ),
                                    );
                                    if (ok == true) {
                                      _markIngredientDone(idx);
                                    }
                                  }
                                },
                                tooltip: (withinTol || allowWithoutWeight)
                                    ? 'Als erledigt markieren'
                                    : 'Außerhalb der Toleranz — Bestätigung nötig',
                              )
                            else
                              Icon(Icons.circle_outlined, color: AppTheme.green),
                          ],
                        ),
                      ),
                    ),
                  );
                },
              );
            }),
          ],
        ),
      ),
    );
  }
}
