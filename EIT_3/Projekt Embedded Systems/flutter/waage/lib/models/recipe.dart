import 'package:hive/hive.dart';

part 'recipe.g.dart';

/// Rezept-Modell mit Zutaten und optionalen Zubereitungsschritten
@HiveType(typeId: 0)
class Recipe {
  @HiveField(0)
  final String name; // Name des Rezepts

  @HiveField(1)
  final int servings; // Anzahl Portionen

  @HiveField(2)
  final List<Ingredient> ingredients; // Liste der Zutaten

  @HiveField(3)
  final String? description; // Optionale Beschreibung

  @HiveField(4)
  final List<String> steps; // Zubereitungsschritte (zwischen/nach Zutaten)

  Recipe({
    required this.name,
    required this.servings,
    required this.ingredients,
    this.description,
    this.steps = const [],
  });
}

/// Einzelne Zutat im Rezept
@HiveType(typeId: 1)
class Ingredient {
  @HiveField(0)
  final String name; // Name der Zutat

  @HiveField(1)
  final double amount; // Menge

  @HiveField(2)
  final String unit; // Einheit (g, ml, Stk, etc.)

  Ingredient({
    required this.name,
    required this.amount,
    required this.unit,
  });
}
