1.1 Descriere
În această temă trebuie să simulați o stație de încărcare pentru un vehicul electric, folosind mai multe LED-uri și butoane. În cadrul acestui task trebuie să țineți cont de stările butonului și să folosiți debouncing, dar și să coordonați toate componentele ca într-un scenariu din viața reală.

1.2 Componente
4x LED-uri (pentru a simula procentul de încărcare)
1x LED RGB (pentru starea de liber sau ocupat)
2x Butoane (pentru start încărcare și stop încărcare)
9x Rezistoare (6x 220/330ohm, 2x 1K)
Breadboard
Linii de legătură
Arduino uno

1.3 Cerinte
Starea stației este ‘liberă’. Loader-ul este stins, iar led-ul pentru disponibilitate este verde.

Se apasă butonul pentru start.

Led-ul pentru disponibilitate se face roșu, iar încărcarea începe prin aprinderea primului LED L1.

Led-ul 1 clipește timp de 3s, celelalte fiind stinse.

Dupa încărcarea primului procent de 25% led-ul rămâne aprins și se trece la următorul led, care va începe să clipească.

La finalizarea încărcării toate led-urile vor clipi simultan de 3 ori, iar apoi se vor stinge, pentru a semnaliza finalizarea procesului.

Led-ul pentru disponibilitate se face verde.

Dacă oricând de la pornirea încărcării până la finalizarea acesteia este apăsat lung (min 1s) butonul de stop, încărcarea se întrerupe prin animația de final (toate led-urile clipesc de 3 ori), iar led-ul pentru disponibilitate devine verde.

![setup_fizic](https://github.com/user-attachments/assets/ea43bbe4-d8d6-41c0-bdfe-95439dea96e2)
![setup_TinkerCAD](https://github.com/user-attachments/assets/ed94e2f6-dfcd-4b13-9d0d-8415c4ffc556)

https://github.com/user-attachments/assets/c5b3c788-0c55-4887-985c-1970dffb8fcf


