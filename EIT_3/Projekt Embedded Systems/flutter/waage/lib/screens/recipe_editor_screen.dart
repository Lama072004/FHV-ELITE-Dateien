import 'package:flutter/material.dart';
import 'package:hive/hive.dart';
import '../theme/app_theme.dart';
import '../models/recipe.dart';

class RecipeEditorScreen extends StatefulWidget {
  final Recipe? recipe;
  final int? index;

  const RecipeEditorScreen({super.key, this.recipe, this.index});

  @override
  State<RecipeEditorScreen> createState() => _RecipeEditorScreenState();
}

class _RecipeEditorScreenState extends State<RecipeEditorScreen> {
  late TextEditingController nameController;
  late TextEditingController servingsController;
  late TextEditingController descriptionController;
  List<Ingredient> ingredients = [];

  @override
  void initState() {
    super.initState();
    nameController = TextEditingController(text: widget.recipe?.name ?? '');
    servingsController =
        TextEditingController(text: widget.recipe?.servings.toString() ?? '1');
    descriptionController =
        TextEditingController(text: widget.recipe?.description ?? '');
    ingredients = List.from(widget.recipe?.ingredients ?? []);
  }

  @override
  void dispose() {
    nameController.dispose();
    servingsController.dispose();
    descriptionController.dispose();
    super.dispose();
  }

  void _addIngredient() {
    showDialog(
      context: context,
      builder: (context) => _IngredientDialog(
        onAdd: (ingredient) {
          setState(() => ingredients.add(ingredient));
          Navigator.pop(context);
        },
      ),
    );
  }

  void _saveRecipe() {
    if (nameController.text.isEmpty) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Bitte einen Rezeptnamen eingeben')),
      );
      return;
    }

    final recipe = Recipe(
      name: nameController.text,
      servings: int.tryParse(servingsController.text) ?? 1,
      ingredients: ingredients,
      description: descriptionController.text,
    );

    try {
      final box = Hive.box<Recipe>('recipes');
      if (widget.index != null) {
        box.putAt(widget.index!, recipe);
        print('Rezept aktualisiert: ${recipe.name}');
      } else {
        final key = box.add(recipe);
        print('Rezept gespeichert mit key: $key, Name: ${recipe.name}');
        print('Aktuelle Anzahl Rezepte in Box: ${box.length}');
      }
      final count = box.length;

      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(
          content: Text('Rezept "${recipe.name}" gespeichert ($count insgesamt)'),
          backgroundColor: AppTheme.green,
        ),
      );

      Future.delayed(const Duration(milliseconds: 500), () {
        if (mounted) Navigator.pop(context);
      });
    } catch (e) {
      print('Fehler beim Speichern: $e');
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(
          content: Text('Fehler beim Speichern: $e'),
          backgroundColor: AppTheme.red,
        ),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text(widget.recipe == null ? 'Rezept erstellen' : 'Rezept bearbeiten'),
      ),
      body: SingleChildScrollView(
        padding: const EdgeInsets.all(16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            TextField(
              controller: nameController,
              decoration: const InputDecoration(
                labelText: 'Rezeptname',
                hintText: 'z.B. Spaghetti Bolognese',
              ),
              style: const TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
            ),
            const SizedBox(height: 16),
            Row(
              children: [
                Expanded(
                  child: TextField(
                    controller: servingsController,
                    decoration: const InputDecoration(labelText: 'Portionen'),
                    keyboardType: TextInputType.number,
                  ),
                ),
              ],
            ),
            const SizedBox(height: 16),
            TextField(
              controller: descriptionController,
              decoration: const InputDecoration(
                labelText: 'Beschreibung (optional)',
                hintText: 'Notizen zum Rezept...',
              ),
              maxLines: 3,
            ),
            const SizedBox(height: 24),
            Text(
              'Zutaten',
              style: Theme.of(context).textTheme.titleMedium?.copyWith(
                    color: AppTheme.lightGreen,
                    fontWeight: FontWeight.bold,
                  ),
            ),
            const SizedBox(height: 12),
            ...ingredients.asMap().entries.map((e) {
              final idx = e.key;
              final ingredient = e.value;
              return Padding(
                padding: const EdgeInsets.only(bottom: 8),
                child: Container(
                  decoration: BoxDecoration(
                    color: const Color(0xFF2A2A2A),
                    borderRadius: BorderRadius.circular(8),
                  ),
                  padding: const EdgeInsets.all(12),
                  child: Row(
                    children: [
                      Expanded(
                        child: Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            Text(ingredient.name,
                                style: const TextStyle(fontWeight: FontWeight.bold)),
                            Text(
                                '${ingredient.amount} ${ingredient.unit}',
                                style: const TextStyle(
                                    color: AppTheme.green, fontSize: 12)),
                          ],
                        ),
                      ),
                      IconButton(
                        icon: const Icon(Icons.delete, color: AppTheme.red),
                        onPressed: () {
                          setState(() => ingredients.removeAt(idx));
                        },
                      ),
                    ],
                  ),
                ),
              );
            }),
            const SizedBox(height: 12),
            ElevatedButton.icon(
              onPressed: _addIngredient,
              icon: const Icon(Icons.add),
              label: const Text('Zutat hinzufügen'),
            ),
            const SizedBox(height: 24),
            ElevatedButton(
              onPressed: _saveRecipe,
              child: const Text('Speichern'),
            ),
          ],
        ),
      ),
    );
  }
}

class _IngredientDialog extends StatefulWidget {
  final Function(Ingredient) onAdd;

  const _IngredientDialog({required this.onAdd});

  @override
  State<_IngredientDialog> createState() => __IngredientDialogState();
}

class __IngredientDialogState extends State<_IngredientDialog> {
  late TextEditingController nameController;
  late TextEditingController amountController;
  String unit = 'g';

  @override
  void initState() {
    super.initState();
    nameController = TextEditingController();
    amountController = TextEditingController();
  }

  @override
  void dispose() {
    nameController.dispose();
    amountController.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      backgroundColor: const Color(0xFF1E1E1E),
      title: const Text('Zutat hinzufügen'),
      content: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          TextField(
            controller: nameController,
            decoration: const InputDecoration(labelText: 'Zutatname'),
          ),
          const SizedBox(height: 12),
          Row(
            children: [
              Expanded(
                child: TextField(
                  controller: amountController,
                  decoration: const InputDecoration(labelText: 'Menge'),
                  keyboardType: TextInputType.number,
                ),
              ),
              const SizedBox(width: 12),
              DropdownButton<String>(
                value: unit,
                items: ['g', 'ml', 'EL', 'TL', 'Stk', 'Tasse']
                    .map((e) => DropdownMenuItem(value: e, child: Text(e)))
                    .toList(),
                onChanged: (v) => setState(() => unit = v ?? 'g'),
              ),
            ],
          ),
        ],
      ),
      actions: [
        TextButton(
          onPressed: () => Navigator.pop(context),
          child: const Text('Abbrechen'),
        ),
        ElevatedButton(
          onPressed: () {
            if (nameController.text.isEmpty) {
              ScaffoldMessenger.of(context).showSnackBar(
                const SnackBar(content: Text('Bitte Zutatname eingeben')),
              );
              return;
            }
            final ingredient = Ingredient(
              name: nameController.text,
              amount: double.tryParse(amountController.text) ?? 0,
              unit: unit,
            );
            widget.onAdd(ingredient);
          },
          child: const Text('Hinzufügen'),
        ),
      ],
    );
  }
}
