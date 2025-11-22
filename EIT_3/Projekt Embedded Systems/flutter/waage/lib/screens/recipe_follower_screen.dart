import 'package:flutter/material.dart';
import '../theme/app_theme.dart';
import '../models/recipe.dart';
import '../services/weight_service.dart';
import '../widgets/weight_gauge.dart';

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

  // track which ingredient indices the user marked as done
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

  Future<void> _onWeightChanged(double? weight, Ingredient? target) async {
    if (target == null) return;
    if (weight == null) {
      _completedShown = false;
      return;
    }
    final tol = 5.0; // gramm
    final minW = target.amount - tol;
    final maxW = target.amount + tol;

    if (weight >= minW && weight <= maxW) {
      if (!_completedShown) {
        _completedShown = true;
        // only show congrats automatically if desired; we now let user confirm
        // await showDialog(...); // removed automatic dialog
      }
    } else {
      _completedShown = false;
    }
  }

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

  // Units that may be marked without exact weight: counts, volume spoons, and common mass/volume units
  bool isMarkAllowedWithoutWeight(String u) {
    final norm = u.toLowerCase().replaceAll(RegExp(r'[^a-z0-9]'), '');
    const allowed = {
      'stk', 'stuck', 'stück', 'stücke', 'pcs', 'piece', 'tasse', 'cup',
      'el', 'esslffel', 'essloffel', 'el.', 'tl', 'teelffel', 'teelloefel', 'tl.',
      // mass/volume units
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
            Text(
              widget.recipe.name,
              style: Theme.of(context).textTheme.headlineSmall?.copyWith(
                    color: AppTheme.lightGreen,
                    fontWeight: FontWeight.bold,
                  ),
            ),
            const SizedBox(height: 24),

            // Gauge with live weight
            Center(
              child: ValueListenableBuilder<double?>(
                valueListenable: WeightService.currentWeight,
                builder: (context, weight, _) {
                  // notify completion check (keeps internal flag)
                  _onWeightChanged(weight, selectedIng);
                  return WeightGauge(
                    currentWeight: weight,
                    maxWeight: (selectedIng?.amount ?? 500),
                    isRecipeMode: true,
                    targetWeight: selectedIng?.amount,
                    tolerance: 5.0, // percent mode kept as before or could be grams
                  );
                },
              ),
            ),
            const SizedBox(height: 12),

            // Live weight text
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

            // Portionen (unchanged)
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

            // Zutatenliste: jedes Item reagiert auf aktuelles Gewicht
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

                  // allow immediate marking if unit is in allowed list (counts, EL/TL, g/ml etc.)
                  final allowWithoutWeight = isMarkAllowedWithoutWeight(ing.unit);

                  return GestureDetector(
                    onTap: () => setState(() => selectedIngredientIndex = idx),
                    child: Padding(
                      padding: const EdgeInsets.only(bottom: 12),
                      child: Container(
                        decoration: BoxDecoration(
                          color: const Color(0xFF2A2A2A),
                          borderRadius: BorderRadius.circular(8),
                          // only green frame when selected
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
                            // Trailing: show done check or mark button when within tolerance or allowed by unit
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
                                    // confirm manual override
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
                                    : 'Außerhalb der Toleranz — lang drücken zum Bestätigen',
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
