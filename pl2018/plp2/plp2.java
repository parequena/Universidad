import java.io.RandomAccessFile;
import java.io.FileNotFoundException;

class plp2
{
    public static void main(String[] args)
    {
    	args = new String[1];
    	args[0] = "/home/pablo/Descargas/pl/autocorrector-plp2/fuentes/p02.txt";

	if (args.length == 1)
	{
	    try {
		RandomAccessFile entrada = new RandomAccessFile(args[0],"r");
		AnalizadorLexico al = new AnalizadorLexico(entrada);
		AnalizadorSintacticoSLR aslr = new AnalizadorSintacticoSLR(al);

		aslr.analizar();
	    }
	    catch (FileNotFoundException e) {
		System.out.println("Error, fichero no encontrado: " + args[0]);
	    }
	} 
	else System.out.println("Error, uso: java plp2 <nomfichero>");
    }
}
