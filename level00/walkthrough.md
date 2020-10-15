le scanf (+74) attend une string et a pour premier argument %d, il attend une valeur décimale
ensuite on a une comparaison avec '0x149c' (5276 en décimal) en +83, si la comparaison est valide, on a un appel système de /bin/sh (0x8048649) et level01 étant le propriétaire de l'exécutable ./level00, on ouvre son terminal avec ses droits.
