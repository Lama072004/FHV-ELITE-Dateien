import 'package:flutter/material.dart';
import 'package:hive_flutter/hive_flutter.dart';
import 'dart:io';
import '../theme/app_theme.dart';
import '../models/recipe.dart';
import '../services/recipe_io_service.dart';
import 'recipe_editor_screen.dart';
import 'recipe_detail_screen.dart';

class RecipesScreen extends StatefulWidget {
  const RecipesScreen({super.key});

  @override
  State<RecipesScreen> createState() => _RecipesScreenState();
}

class _RecipesScreenState extends State<RecipesScreen> {
  late Box<Recipe> recipesBox;

  @override
  void initState() {
    super.initState();
    recipesBox = Hive.box<Recipe>('recipes');
    print('RecipesScreen initState: ${recipesBox.length} Rezepte in Box');
  }

  Future<void> _importRecipe() async {
    try {
      // Öffne Datei-Dialog via native Intent (Android)
      final result = await _pickRcpeFile();
      if (result == null) return;

      final file = File(result);
      final info = await RecipeIOService.getFileInfo(file);

      if (!mounted) return;

      showDialog(
        context: context,
        builder: (context) => AlertDialog(
          backgroundColor: const Color(0xFF1E1E1E),
          title: const Text('Rezept importieren?'),
          content: Text(info),
          actions: [
            TextButton(
              onPressed: () => Navigator.pop(context),
              child: const Text('Abbrechen'),
            ),
            ElevatedButton(
              onPressed: () async {
                try {
                  final recipe = await RecipeIOService.importRecipe(file);
                  recipesBox.add(recipe);

                  if (!mounted) return;
                  Navigator.pop(context);

                  ScaffoldMessenger.of(context).showSnackBar(
                    SnackBar(
                      content: Text('Rezept "${recipe.name}" importiert'),
                      backgroundColor: AppTheme.green,
                    ),
                  );
                } catch (e) {
                  if (!mounted) return;
                  Navigator.pop(context);

                  ScaffoldMessenger.of(context).showSnackBar(
                    SnackBar(
                      content: Text('Fehler: $e'),
                      backgroundColor: AppTheme.red,
                    ),
                  );
                }
              },
              child: const Text('Importieren'),
            ),
          ],
        ),
      );
    } catch (e) {
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(
          content: Text('Fehler beim Importieren: $e'),
          backgroundColor: AppTheme.red,
        ),
      );
    }
  }

  // Stub-Implementierung für File-Picker (kann per Intent erweitert werden)
  Future<String?> _pickRcpeFile() async {
    // Placeholder: In echtem Projekt würde man hier via Android-Intent eine Datei wählen
    // Für Demo: Rückgabe null (benutzer kann später über Share-Intent Rezepte empfangen)
    return null;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: ValueListenableBuilder<Box>(
        valueListenable: recipesBox.listenable(),
        builder: (context, Box box, _) {
          print('ValueListenableBuilder rebuild: ${box.length} Rezepte');
          if (box.isEmpty) {
            return Center(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  Icon(Icons.menu_book,
                      size: 64, color: AppTheme.green.withOpacity(0.5)),
                  const SizedBox(height: 16),
                  const Text('Keine Rezepte vorhanden'),
                  const SizedBox(height: 24),
                  ElevatedButton.icon(
                    onPressed: () {
                      Navigator.push(
                        context,
                        MaterialPageRoute(
                          builder: (_) => const RecipeEditorScreen(),
                        ),
                      );
                    },
                    icon: const Icon(Icons.add),
                    label: const Text('Rezept erstellen'),
                  ),
                ],
              ),
            );
          }

          final recipes = box.values.cast<Recipe>().toList();
          return ListView(
            padding: const EdgeInsets.all(16),
            children: [
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  Text(
                    'Meine Rezepte (${recipes.length})',
                    style: Theme.of(context).textTheme.titleLarge?.copyWith(
                          color: AppTheme.lightGreen,
                          fontWeight: FontWeight.bold,
                        ),
                  ),
                  IconButton(
                    icon: const Icon(Icons.download),
                    onPressed: _importRecipe,
                    tooltip: 'Rezept importieren',
                  ),
                ],
              ),
              const SizedBox(height: 12),
              ...recipes.asMap().entries.map((e) {
                final index = e.key;
                final recipe = e.value;
                return _buildRecipeCard(context, recipe, index);
              }),
            ],
          );
        },
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () {
          Navigator.push(
            context,
            MaterialPageRoute(
              builder: (_) => const RecipeEditorScreen(),
            ),
          );
        },
        child: const Icon(Icons.add),
      ),
    );
  }

  Widget _buildRecipeCard(BuildContext context, Recipe recipe, int index) {
    return GestureDetector(
      onTap: () {
        Navigator.push(
          context,
          MaterialPageRoute(
            builder: (_) => RecipeDetailScreen(recipe: recipe, index: index),
          ),
        );
      },
      onLongPress: () {
        showModalBottomSheet(
          context: context,
          builder: (context) => Container(
            color: const Color(0xFF2A2A2A),
            child: Column(
              mainAxisSize: MainAxisSize.min,
              children: [
                ListTile(
                  leading: const Icon(Icons.download, color: AppTheme.green),
                  title: const Text('Exportieren'),
                  onTap: () async {
                    Navigator.pop(context);
                    try {
                      final file =
                          await RecipeIOService.exportRecipe(recipe);
                      ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(
                          content:
                              Text('Gespeichert: ${file.path}'),
                          backgroundColor: AppTheme.green,
                        ),
                      );
                    } catch (e) {
                      ScaffoldMessenger.of(context).showSnackBar(
                        SnackBar(
                          content: Text('Fehler: $e'),
                          backgroundColor: AppTheme.red,
                        ),
                      );
                    }
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.edit, color: AppTheme.green),
                  title: const Text('Bearbeiten'),
                  onTap: () {
                    Navigator.pop(context);
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (_) => RecipeEditorScreen(
                          recipe: recipe,
                          index: index,
                        ),
                      ),
                    );
                  },
                ),
                ListTile(
                  leading: const Icon(Icons.delete, color: AppTheme.red),
                  title: const Text('Löschen'),
                  onTap: () {
                    Navigator.pop(context);
                    showDialog(
                      context: context,
                      builder: (context) => AlertDialog(
                        backgroundColor: const Color(0xFF1E1E1E),
                        title: const Text('Rezept löschen?'),
                        content:
                            const Text('Dieses Rezept wird gelöscht'),
                        actions: [
                          TextButton(
                            onPressed: () => Navigator.pop(context),
                            child: const Text('Abbrechen'),
                          ),
                          ElevatedButton(
                            onPressed: () {
                              recipesBox.deleteAt(index);
                              Navigator.pop(context);
                              ScaffoldMessenger.of(context).showSnackBar(
                                const SnackBar(
                                  content: Text('Rezept gelöscht'),
                                  backgroundColor: AppTheme.red,
                                ),
                              );
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
          ),
        );
      },
      child: Card(
        color: const Color(0xFF2A2A2A),
        margin: const EdgeInsets.only(bottom: 12),
        shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
        child: Padding(
          padding: const EdgeInsets.all(16),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Text(
                recipe.name,
                style: Theme.of(context).textTheme.titleLarge?.copyWith(
                      fontWeight: FontWeight.bold,
                      color: AppTheme.lightGreen,
                    ),
              ),
              const SizedBox(height: 8),
              Row(
                children: [
                  const Icon(Icons.people, size: 16, color: AppTheme.green),
                  const SizedBox(width: 4),
                  Text('${recipe.servings} Portionen',
                      style: const TextStyle(color: AppTheme.green)),
                  const SizedBox(width: 16),
                  const Icon(Icons.category, size: 16, color: AppTheme.green),
                  const SizedBox(width: 4),
                  Text('${recipe.ingredients.length} Zutaten',
                      style: const TextStyle(color: AppTheme.green)),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }
}
