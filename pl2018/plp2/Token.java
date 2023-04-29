import java.util.ArrayList;

public class Token {
	public static char EndOfFile = '';

	public int fila;
	public int columna;

	public String lexema;
	public static final ArrayList<String> nombreToken = new ArrayList<String>();

	static {
		nombreToken.add("(");
		nombreToken.add(")");
		nombreToken.add(","); // Commented on plp2
		nombreToken.add("&"); // Commented on plp2
		nombreToken.add("{");
		nombreToken.add("}");
		nombreToken.add("=");
		nombreToken.add(";");
		nombreToken.add("+ -");
		nombreToken.add("'int'");
		nombreToken.add("'float'");
		nombreToken.add("'if'");
		nombreToken.add("identificador");
		nombreToken.add("numero entero");
		nombreToken.add("numero real");
		nombreToken.add("fin de fichero");
	}

	public int tipo; // Tipo es: ID, ENTERO, REAL ...

	public static final int PARI = 0, PARD = 1,
			COMA = 2, AMP = 3, LLAVEI = 4, LLAVED = 5, ASIG = 6, PYC = 7, OPAS = 8, INT =
			9, FLOAT = 10, IF = 11, ID = 12, ENTERO = 13, REAL = 14, EOF = 15;

	/*LLAVEI = 2, LLAVED = 3, ASIG = 4, PYC = 5, OPAS = 6, INT = 7, FLOAT = 8, IF = 9, ID = 10, ENTERO = 11,
	    REAL = 12, EOF = 13;*/

	public String toString() {
		return nombreToken.get(tipo);
	}

	// Ctor.
	public Token(int _f, int _c, String _l, int _t) {
		fila = _f;
		columna = _c;
		lexema = _l;
		tipo = _t;
	}
	
	public void setType(int _t)
	{
		tipo = _t;
	}
	
	public void setLexeme(String _s)
	{
		lexema = _s;
	}


}
