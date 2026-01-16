// Der folgende ifdef-Block ist die Standardmethode zum Erstellen von Makros, die das Exportieren
// aus einer DLL vereinfachen. Alle Dateien in dieser DLL werden mit dem BINK2W64_EXPORTS-Symbol
// (in der Befehlszeile definiert) kompiliert. Dieses Symbol darf für kein Projekt definiert werden,
// das diese DLL verwendet. Alle anderen Projekte, deren Quelldateien diese Datei beinhalten, sehen
// BINK2W64_API-Funktionen als aus einer DLL importiert an, während diese DLL
// mit diesem Makro definierte Symbole als exportiert ansieht.
#ifdef BINK2W64_EXPORTS
#define BINK2W64_API __declspec(dllexport)
#else
#define BINK2W64_API __declspec(dllimport)
#endif

// Diese Klasse wird aus der DLL exportiert.
class BINK2W64_API Cbink2w64 {
public:
	Cbink2w64(void);
	// TODO: Methoden hier hinzufügen.
};

extern BINK2W64_API int nbink2w64;

BINK2W64_API int fnbink2w64(void);
