Attribute VB_Name = "StoerschriebInDictArray"
Option Explicit

Public Function DataInArray(ByVal filePath As String) As Variant
    '------------------------------------------------------------
    'Vordefinitionen
    '------------------------------------------------------------
    'Dictionary initialisieren
    Dim dict As Object
    Set dict = CreateObject("Scripting.Dictionary")
    'fso ist für das Einlesen der Datei
    Dim fso As Object
    Set fso = CreateObject("Scripting.FileSystemObject")
    'ts ist für das Zeilenweise Einlesen der Datei
    Dim ts As Object
    Set ts = fso.OpenTextFile(filePath, 1)
    '1 = For Reading
    '2 = For Writing
    '8 = For Appending
    
    Dim line As String
    Dim ZeilenTeile() As String
    Dim Trennzeichen As String
    Dim i As Long
    
    Dim startLine As Long
    startLine = 3
    Dim currentLine As Long
    currentLine = 0

    
    ' Keys (Spaltenname) für Dict mit Arrays initialisiert
    dict.Add "ID-A", Array()
    dict.Add "B", Array()
    dict.Add "C", Array()
    dict.Add "D", Array()
    dict.Add "E", Array()
    dict.Add "F", Array()
    dict.Add "G", Array()
    dict.Add "H", Array()
    dict.Add "I", Array()
    dict.Add "J", Array()
    dict.Add "K", Array()
    dict.Add "L", Array()
    dict.Add "M", Array()
    
    '------------------------------------------------------------
    'Logikteil
    '------------------------------------------------------------
    Do While Not ts.AtEndOfStream
        line = ts.ReadLine
        currentLine = currentLine + 1
        line = Replace(line, ";", ",")
        'sobald currentLine einen Wert von der StartLine erreicht hat wird das Eintragen in das Dictionary gestartet
        If currentLine >= startLine Then
            'Zeilen splitten
            ZeilenTeile = Split(line, ",")
            
            Dim maxcols As Long
            maxcols = dict.Count
            'Werte in Dictionary schreiben
            For i = 0 To maxcols - 1
                If i <= UBound(ZeilenTeile) Then
                    Select Case i
                        Case 0: dict("ID-A") = AppendToArray(dict("ID-A"), ZeilenTeile(i))
                        Case 1: dict("B") = AppendToArray(dict("B"), ZeilenTeile(i))
                        Case 2: dict("C") = AppendToArray(dict("C"), ZeilenTeile(i))
                        Case 3: dict("D") = AppendToArray(dict("D"), ZeilenTeile(i))
                        Case 4: dict("E") = AppendToArray(dict("E"), ZeilenTeile(i))
                        Case 5: dict("F") = AppendToArray(dict("F"), ZeilenTeile(i))
                        Case 6: dict("G") = AppendToArray(dict("G"), ZeilenTeile(i))
                        Case 7: dict("H") = AppendToArray(dict("H"), ZeilenTeile(i))
                        Case 8: dict("I") = AppendToArray(dict("I"), ZeilenTeile(i))
                        Case 9: dict("J") = AppendToArray(dict("J"), ZeilenTeile(i))
                        Case 10: dict("K") = AppendToArray(dict("K"), ZeilenTeile(i))
                        Case 11: dict("L") = AppendToArray(dict("L"), ZeilenTeile(i))
                        Case 12: dict("M") = AppendToArray(dict("M"), ZeilenTeile(i))
                        ' zusätzliche Spalten werden ignoriert
                    End Select
                Else
                    Select Case i
                        Case 0: dict("ID-A") = AppendToArray(dict("ID-A"), "#")
                        Case 1: dict("B") = AppendToArray(dict("B"), "#")
                        Case 2: dict("C") = AppendToArray(dict("C"), "#")
                        Case 3: dict("D") = AppendToArray(dict("D"), "#")
                        Case 4: dict("E") = AppendToArray(dict("E"), "#")
                        Case 5: dict("F") = AppendToArray(dict("F"), "#")
                        Case 6: dict("G") = AppendToArray(dict("G"), "#")
                        Case 7: dict("H") = AppendToArray(dict("H"), "#")
                        Case 8: dict("I") = AppendToArray(dict("I"), "#")
                        Case 9: dict("J") = AppendToArray(dict("J"), "#")
                        Case 10: dict("K") = AppendToArray(dict("K"), "#")
                        Case 11: dict("L") = AppendToArray(dict("L"), "#")
                        Case 12: dict("M") = AppendToArray(dict("M"), "#")
                    End Select
                End If
            Next i
        End If
    Loop
    ts.Close
    'Dictionary zurückgeben
    Set DataInArray = dict
End Function


Private Function AppendToArray(original As Variant, newItem As Variant) As Variant
    Dim n As Long
    n = UBound(original) + 1
    ReDim Preserve original(0 To n)
    original(n) = newItem
    AppendToArray = original
End Function
