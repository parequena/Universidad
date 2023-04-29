import java.io.RandomAccessFile;
import java.io.FileNotFoundException;
import java.io.IOException;

class plp1 {
    public static void main(String[] args) {

        /*if (args.length == 1)
        {*/
          try {
            RandomAccessFile entrada = new RandomAccessFile("/home/pablo/Descargas/pl/autocorrector-plp1/fuentes/p01.txt","r");
            AnalizadorLexico al = new AnalizadorLexico(entrada);
            AnalizadorSintacticoDR asdr = new AnalizadorSintacticoDR(al);

            asdr.Fun(); // simbolo inicial de la gramatica
            asdr.comprobarFinFichero();
          }
          catch (FileNotFoundException e) {
            System.out.println("Error, fichero no encontrado: " + args[0]);
          }
       /* } 
        else System.out.println("Error, uso: java plp1 <nomfichero>");*/
    }
}
