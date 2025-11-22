import 'dart:convert';
import 'dart:io';
import 'package:crypto/crypto.dart';
import 'package:path_provider/path_provider.dart';
import '../models/recipe.dart';

class RecipeFormatService {
  static const String _magicNumber = "RCPE";
  static const int _version = 1;

  static Future<File> exportRecipe(Recipe recipe) async {
    try {
      final recipeMap = _recipeToMap(recipe);
      final jsonString = jsonEncode(recipeMap);
      final checksum = sha256.convert(utf8.encode(jsonString)).toString();
      final fileContent =
          _magicNumber + String.fromCharCode(_version) + checksum + jsonString;

      final directory = await getTemporaryDirectory();
      final fileName =
          '${recipe.name.replaceAll(' ', '_')}_${DateTime.now().millisecondsSinceEpoch}.rcpe';
      final file = File('${directory.path}/$fileName');

      await file.writeAsString(fileContent);
      return file;
    } catch (e) {
      throw Exception('Fehler beim Exportieren: $e');
    }
  }

  static Future<Recipe> importRecipe(File file) async {
    try {
      final content = await file.readAsString();

      if (!content.startsWith(_magicNumber)) {
        throw Exception('Ungültiges Dateiformat. Ist das eine .rcpe Datei?');
      }

      final version = content.codeUnitAt(4);
      if (version != _version) {
        throw Exception('Nicht unterstützte Dateiformat-Version: $version');
      }

      final storedChecksum = content.substring(5, 5 + 64);
      final jsonString = content.substring(5 + 64);

      final calculatedChecksum =
          sha256.convert(utf8.encode(jsonString)).toString();
      if (storedChecksum != calculatedChecksum) {
        throw Exception(
            'Datei beschädigt oder manipuliert! Checksumme stimmt nicht überein.');
      }

      final recipeMap = jsonDecode(jsonString) as Map<String, dynamic>;
      return _mapToRecipe(recipeMap);
    } catch (e) {
      throw Exception('Fehler beim Importieren: $e');
    }
  }

  static Map<String, dynamic> _recipeToMap(Recipe recipe) {
    return {
      'format_version': _version,
      'export_date': DateTime.now().toIso8601String(),
      'name': recipe.name,
      'description': recipe.description,
      'servings': recipe.servings,
      'ingredients': recipe.ingredients
          .map((ing) => {
                'name': ing.name,
                'amount': ing.amount,
                'unit': ing.unit,
              })
          .toList(),
    };
  }

  static Recipe _mapToRecipe(Map<String, dynamic> map) {
    final ingredientsList = (map['ingredients'] as List<dynamic>)
        .map((ing) => Ingredient(
              name: ing['name'] as String,
              amount: (ing['amount'] as num).toDouble(),
              unit: ing['unit'] as String,
            ))
        .toList();

    return Recipe(
      name: map['name'] as String,
      servings: map['servings'] as int,
      ingredients: ingredientsList,
      description: map['description'] as String?,
    );
  }

  static Future<String> getFileInfo(File file) async {
    try {
      final content = await file.readAsString();
      final jsonString = content.substring(5 + 64);
      final recipeMap = jsonDecode(jsonString) as Map<String, dynamic>;

      final name = recipeMap['name'] as String;
      final servings = recipeMap['servings'] as int;
      final ingredientsCount = (recipeMap['ingredients'] as List).length;
      final exportDate = recipeMap['export_date'] as String;

      return '''
Rezeptname: $name
Portionen: $servings
Zutaten: $ingredientsCount
Exportiert: $exportDate
''';
    } catch (e) {
      return 'Fehler beim Lesen der Datei: $e';
    }
  }
}
