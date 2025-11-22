import 'package:hive/hive.dart';

part 'recipe.g.dart';

@HiveType(typeId: 0)
class Recipe {
  @HiveField(0)
  final String name;

  @HiveField(1)
  final int servings;

  @HiveField(2)
  final List<Ingredient> ingredients;

  @HiveField(3)
  final String? description;

  Recipe({
    required this.name,
    required this.servings,
    required this.ingredients,
    this.description,
  });
}

@HiveType(typeId: 1)
class Ingredient {
  @HiveField(0)
  final String name;

  @HiveField(1)
  final double amount;

  @HiveField(2)
  final String unit;

  Ingredient({
    required this.name,
    required this.amount,
    required this.unit,
  });
}
