import 'package:flutter/material.dart';
import 'package:hive/hive.dart';
import '../theme/app_theme.dart';
import '../models/recipe.dart';
import '../services/recipe_io_service.dart';
import 'recipe_editor_screen.dart';
import 'recipe_follower_screen.dart';

class RecipeDetailScreen extends StatefulWidget {
  final Recipe recipe;
  final int index;

  const RecipeDetailScreen({
    required this.recipe,
    required this.index,
    super.key,
  });

  @override
  State<RecipeDetailScreen> createState() => _RecipeDetailScreenState();
}

class _RecipeDetailScreenState extends State<RecipeDetailScreen> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Rezeptdetails'),
        actions: [
          IconButton(
            icon: const Icon(Icons.edit),
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(
                  builder: (_) => RecipeEditorScreen(
                    recipe: widget.recipe,
                    index: widget.index,
                  ),
                ),
              );
            },
          ),
          IconButton(
            icon: const Icon(Icons.download, color: AppTheme.green),
            onPressed: () async {
              try {
                final file = await RecipeIOService.exportRecipe(widget.recipe);
                ScaffoldMessenger.of(context).showSnackBar(
                  SnackBar(
                    content: Text('Gespeichert: ${file.path}'),
                    backgroundColor: AppTheme.green,
                  ),
                );
              } catch (e) {
                ScaffoldMessenger.of(context).showSnackBar(
                  SnackBar(content: Text('Fehler: $e')),
                );
              }
            },
          ),
          IconButton(
            icon: const Icon(Icons.delete, color: AppTheme.red),
            onPressed: () {
              showDialog(
                context: context,
                builder: (context) => AlertDialog(
                  backgroundColor: const Color(0xFF1E1E1E),
                  title: const Text('Löschen?'),
                  content: const Text('Dieses Rezept wird gelöscht'),
                  actions: [
                    TextButton(
                      onPressed: () => Navigator.pop(context),
                      child: const Text('Abbrechen'),
                    ),
                    ElevatedButton(
                      onPressed: () {
                        Hive.box<Recipe>('recipes').deleteAt(widget.index);
                        Navigator.pop(context);
                        Navigator.pop(context);
                      },
                      style: ElevatedButton.styleFrom(
                        backgroundColor: AppTheme.red,
                      ),
                      child: const Text('Löschen'),
                    ),
                  ],
                ),
              );
            },
          ),
        ],
      ),
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
            const SizedBox(height: 8),
            Row(
              children: [
                const Icon(Icons.people, color: AppTheme.green),
                const SizedBox(width: 8),
                Text('${widget.recipe.servings} Portionen',
                    style: const TextStyle(color: AppTheme.green)),
              ],
            ),
            if (widget.recipe.description?.isNotEmpty ?? false) ...[
              const SizedBox(height: 16),
              Text(
                widget.recipe.description ?? '',
                style: const TextStyle(color: Color(0xFF888888)),
              ),
            ],
            const SizedBox(height: 24),
            Text(
              'Zutaten',
              style: Theme.of(context).textTheme.titleMedium?.copyWith(
                    color: AppTheme.lightGreen,
                    fontWeight: FontWeight.bold,
                  ),
            ),
            const SizedBox(height: 12),
            ...widget.recipe.ingredients.map((ing) => Padding(
              padding: const EdgeInsets.only(bottom: 8),
              child: Row(
                children: [
                  const Icon(Icons.circle, size: 8, color: AppTheme.green),
                  const SizedBox(width: 12),
                  Expanded(child: Text(ing.name)),
                  Text(
                    '${ing.amount} ${ing.unit}',
                    style: const TextStyle(
                      color: AppTheme.lightGreen,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                ],
              ),
            )),
            const SizedBox(height: 32),
            ElevatedButton.icon(
              onPressed: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(
                    builder: (_) =>
                        RecipeFollowerScreen(recipe: widget.recipe),
                  ),
                );
              },
              icon: const Icon(Icons.play_arrow),
              label: const Text('Rezept folgen'),
            ),
          ],
        ),
      ),
    );
  }
}
