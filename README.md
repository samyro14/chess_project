# chess_project + sdl2 for ui

1
Documentație 
1. Titlu Proiect: Chess game 
 
2. Descriere: 
Acest proiect reprezintă o aplicație completă de șah cu 
interfață grafică, dezvoltată în limbajul C, ce permite jocul între 
doi jucători conectați în rețea (server și client). Interfața este 
realizată cu ajutorul bibliotecilor SDL2, SDL2_image și SDL2_ttf, 
iar comunicarea între jucători se face prin socket-uri TCP folosind 
Winsock2 (Windows). 
 
Jocul oferă o experiență interactivă, cu meniu grafic, introducerea 
numelui jucătorului, validarea mutărilor conform regulilor oficiale 
de șah, afișarea grafică a tablei și pieselor, precum și salvarea 
rezultatelor fiecărei partide într-un fișier text. 
 
Proiectul este destinat atât exersării programării orientate pe 
evenimente și grafice, cât și înțelegerii principiilor de bază ale 
comunicației în rețea și sincronizării între procese. Este ideal 
pentru uz educațional, laborator sau proiecte personale. 
 
 
3. Instalare: 
    1. Instalează dependențele:   
    - SDL2, SDL2_image, SDL2_ttf (prin MSYS2:  `pacman -S
    mingw-w64-i686-SDL2 mingw-w64-i686-SDL2_image mingw-
    w64-i686-SDL2_ttf` ) 
    2. Compilează proiectul: 
    - Folosește Makefile-ul:   
        ```mingw32-make build``` 
        
    3. Pornește serverul:   
    -Deschide un terminal în folderul unde se află jocul:   
        ```./sdl_game.exe server 12345 ```
    
    4. Pornește clientul: 
    - În alt terminal sau pe alt calculator:   
        ```./sdl_game.exe client 127.0.0.1 12345 ```
    - Înlocuiește `127.0.0.1` cu IP-ul serverului dacă nu rulezi local. 
    
4. Utilizare: 
    • Meniu:   
    a.  Introdu numele în caseta de text. 
    b.  Click pe "Play" pentru a începe jocul sau "Exit" pentru a 
    ieși. 
    • Joc: 
    a.  Selectează piesa cu mouse-ul, apoi dă click pe pătratul 
    destinație. 
    b.  Mutările sunt transmise automat adversarului. 
    • Rezultate:   
    a.  La finalul fiecărei partide, rezultatul este salvat în 
    `results.txt`. 
 
 
 
 
6. Structura Proiectului: 
Proiect_chess/ 
│ 
├── assets/ 
│   ├── fonts/ 
│   │   └── arial.ttf 
│   └── img/ 
│       ├── background/ 
│       │   └── menu_background.jpg 
│       └── pieces/ 
│           ├── white/ 
│           │   └── pawn.png, knight.png, ... 
│           └── black/ 
│               └── pawn.png, knight.png, ... 
│ 
├── src/ 
│   ├── main.c 
│   ├── board.c 
│   ├── board.h 
│   ├── moves.c 
│   ├── moves.h 
│   ├── socket_utils.c 
│   ├── socket_utils.h 
│   ├── pawn.c, knight.c, bishop.c, rook.c, queen.c, king.c 
│ 
├── results.txt 
├── Makefile 
└── README.md 
7. Extensii posibile: 
a.  Implementare AI pentru single-player. 
b.  Adăugare sunete și animații. 
c.  Salvare și încărcare automată a partidelor. 
d.  Suport pentru chat între jucători. 
e.  Diferite moduri de joc(blitz, rapid etc). 
f.  Implementarea jocului pe timp. 
g.  Posibilitatea ca user-ul să aleagă piesa dorită atunci când 
pionul ajunge în baza adversarului. 
8. Dependencies 
SDL2, SDL2_image, SDL2_ttf 
Winsock2 (Windows, included with MinGW)