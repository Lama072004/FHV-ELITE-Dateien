import 'dart:convert';
import 'dart:io';
import 'package:crypto/crypto.dart';
import 'package:path_provider/path_provider.dart';
import '../models/recipe.dart';

/// Service für Export/Import von Rezepten im proprietären .rcpe-Format
class RecipeIOService {
  static const String _magicNumber = "RCPE"; // Dateiformat-Identifikator
  static const int _version = 1; // Format-Version

  /// Exportiert ein Rezept als .rcpe Datei in das angegebene Verzeichnis
  /// oder ins Standard-Download-Verzeichnis (Android/Downloads)
  static Future<File> exportRecipe(Recipe recipe, {String? customPath}) async {
    try {
      // Rezept zu Map konvertieren
      final recipeMap = _recipeToMap(recipe);
      final jsonString = jsonEncode(recipeMap);

      // SHA256-Checksumme zur Validierung erstellen
      final checksum = sha256.convert(utf8.encode(jsonString)).toString();

      // Dateiinhalt: Magic Number + Version + Checksumme + JSON
      final fileContent =
          _magicNumber + String.fromCharCode(_version) + checksum + jsonString;

      // Verzeichnis bestimmen
      Directory directory;
      if (customPath != null && customPath.isNotEmpty) {
        directory = Directory(customPath);
        if (!await directory.exists()) {
          await directory.create(recursive: true);
        }
      } else {
        // Standard: Downloads-Ordner (Android) oder Dokumente
        if (Platform.isAndroid) {
          directory = Directory('/storage/emulated/0/Download');
          if (!await directory.exists()) {
            directory = await getApplicationDocumentsDirectory();
          }
        } else {
          directory = await getApplicationDocumentsDirectory();
        }
      }

      final fileName =
          '${recipe.name.replaceAll(' ', '_')}_${DateTime.now().millisecondsSinceEpoch}.rcpe';
      final file = File('${directory.path}/$fileName');

      await file.writeAsString(fileContent);
      print('Rezept exportiert: ${file.path}');
      return file;
    } catch (e) {
      throw Exception('Fehler beim Exportieren: $e');
    }
  }

  /// Importiert ein Rezept aus einer .rcpe Datei
  static Future<Recipe> importRecipe(File file) async {
    try {
      final content = await file.readAsString();

      // Magic Number prüfen
      if (!content.startsWith(_magicNumber)) {
        throw Exception('Ungültiges Dateiformat. Ist das eine .rcpe Datei?');
      }

      // Version prüfen
      final version = content.codeUnitAt(4);
      if (version != _version) {
        throw Exception('Nicht unterstützte Dateiformat-Version: $version');
      }

      // Checksumme auslesen und validieren
      final storedChecksum = content.substring(5, 5 + 64);
      final jsonString = content.substring(5 + 64);

      final calculatedChecksum =
          sha256.convert(utf8.encode(jsonString)).toString();
      if (storedChecksum != calculatedChecksum) {
        throw Exception(
            'Datei beschädigt oder manipuliert! Checksumme stimmt nicht überein.');
      }

      // JSON zu Recipe-Objekt konvertieren
      final recipeMap = jsonDecode(jsonString) as Map<String, dynamic>;
      return _mapToRecipe(recipeMap);
    } catch (e) {
      throw Exception('Fehler beim Importieren: $e');
    }
  }

  /// Konvertiert Recipe zu Map für JSON-Serialisierung
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
      'steps': recipe.steps, // Zubereitungsschritte hinzufügen
    };
  }

  /// Konvertiert Map zu Recipe-Objekt
  static Recipe _mapToRecipe(Map<String, dynamic> map) {
    final ingredientsList = (map['ingredients'] as List<dynamic>)
        .map((ing) => Ingredient(
              name: ing['name'] as String,
              amount: (ing['amount'] as num).toDouble(),
              unit: ing['unit'] as String,
            ))
        .toList();

    final stepsList = map['steps'] != null
        ? (map['steps'] as List<dynamic>).map((s) => s as String).toList()
        : <String>[];

    return Recipe(
      name: map['name'] as String,
      servings: map['servings'] as int,
      ingredients: ingredientsList,
      description: map['description'] as String?,
      steps: stepsList,
    );
  }

  /// Liest Rezept-Infos aus einer .rcpe Datei (ohne vollständigen Import)
  static Future<String> getFileInfo(File file) async {
    try {
      final content = await file.readAsString();
      final jsonString = content.substring(5 + 64);
      final recipeMap = jsonDecode(jsonString) as Map<String, dynamic>;

      final name = recipeMap['name'] as String;
      final servings = recipeMap['servings'] as int;
      final ingredientsCount = (recipeMap['ingredients'] as List).length;
      final stepsCount = recipeMap['steps'] != null ? (recipeMap['steps'] as List).length : 0;

      return '''
Rezept: $name
Portionen: $servings
Zutaten: $ingredientsCount
Schritte: $stepsCount
''';
    } catch (e) {
      return 'Fehler beim Lesen der Datei: $e';
    }
  }
}
