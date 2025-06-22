# IZU Project 2 - Práce s datovými strukturami

## Popis projektu

Program pro vyhledávání geometrických útvarů v monochromatických obrázcích. Aplikace analyzuje bitmapové obrázky uložené jako textové soubory a vyhledává:
- Nejdelší vodorovné úsečky
- Nejdelší svislé úsečky  
- Největší čtverce

## Požadavky

- Zdrojový soubor: `figsearch.c`
- Překlad: `cc -std=c11 -Wall -Wextra -Werror figsearch.c -o figsearch`

## Syntaxe spuštění

```bash
./figsearch --help
./figsearch test SOUBOR
./figsearch hline SOUBOR
./figsearch vline SOUBOR
./figsearch square SOUBOR
```

### Argumenty

- `--help` - zobrazí nápovědu
- `test` - zkontroluje validitu formátu obrázku
- `hline` - najde nejdelší horizontální úsečku
- `vline` - najde nejdelší vertikální úsečku
- `square` - najde největší čtverec

## Formát vstupního souboru

Textový soubor obsahující:
1. Rozměry obrázku (výška šířka)
2. Matici hodnot 0 (bílá) a 1 (černá)

**Příklad:**
```
4 5
0 0 1 1 1
0 0 1 0 1
1 0 1 1 1
1 1 1 1 1
```

## Definice útvarů

- **Horizontální úsečka**: Nepřerušená posloupnost černých pixelů na stejném řádku
- **Vertikální úsečka**: Nepřerušená posloupnost černých pixelů ve stejném sloupci
- **Čtverec**: Obvod čtverce tvořený černými pixely

## Výstupní formát

Souřadnice ve formátu: `R1 C1 R2 C2`
- R1,C1 = počáteční souřadnice (levý horní roh)
- R2,C2 = koncová souřadnice (pravý dolní roh)

Pokud není nalezen žádný útvar: `Not found`

## Příklady použití

```bash
# Nejdelší horizontální úsečka
./figsearch hline obrazek.txt
# Výstup: 3 0 3 4

# Nejdelší vertikální úsečka  
./figsearch vline obrazek.txt
# Výstup: 0 2 3 2

# Největší čtverec
./figsearch square obrazek.txt
# Výstup: 0 2 2 4
```

## Poznámky k implementaci

- Souřadnice začínají od 0,0 (levý horní roh)
- V případě více útvarů stejné velikosti se vrací první nalezený
- "První" znamená s nejmenším indexem řádku, případně sloupce
