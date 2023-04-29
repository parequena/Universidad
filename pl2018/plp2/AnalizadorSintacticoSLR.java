// import java.util.ArrayList;
import java.util.ArrayList;
import java.util.Stack;

public class AnalizadorSintacticoSLR
{	
	///// This block of functions are for adapt the Token class (never change specification please).
	public static final ArrayList<String> tokensPL2 = new ArrayList<String>();
	static {
		tokensPL2.add("(");
		tokensPL2.add(")");
		tokensPL2.add("{");
		tokensPL2.add("}");
		tokensPL2.add("=");
		tokensPL2.add(";");
		tokensPL2.add("+ -");
		tokensPL2.add("'int'");
		tokensPL2.add("'float'");
		tokensPL2.add("'if'");
		tokensPL2.add("identificador");
		tokensPL2.add("numero entero");
		tokensPL2.add("numero real");
		tokensPL2.add("fin de fichero");
	}
	private void updateToken()
	{
		// 0, 1, 2, 3, 4, 5, 6, 7,  8,   9,    10, 11, 12,   13,   14, 15
		// 0, 1,     , 2, 3, 4, 5,  6,   7,     8,  9, 10,   11,   12, 13 //// plp2
		
		if(tok.tipo >= 4)
		{
			int newType = tok.tipo-2;
			tok.setType(newType);
		}
	}
	///// End Block.
	
	// Table position of a left part of a rule Example: 20 from T -> id (rule 15). 'cause T is on the 20th position.
	private static final int[] leftParts =
	{
		/*0 */ 0, // All indeces starts on 1.
		/*1 */ 14,
		/*2 */ 15,
        /*3 */ 15,
		/*4 */ 16,
		/*5 */ 17,
		/*6 */ 17,
		/*7 */ 18,
		/*8 */ 18,
		/*9 */ 18,
		/*10*/ 18,
		/*11*/ 19,
		/*12*/ 19,
		/*13*/ 20,
		/*14*/ 20,
		/*15*/ 20
	};	

	// How many right parts has a rule Example: 1 from T -> id (rule 15). 
	private static final int[] rightParts =
	{
		/*0 */ 0, // All indeces starts on 1.
	    /*1 */ 5,
	    /*2 */ 1,
	    /*3 */ 1,
	    /*4 */ 3,
	    /*5 */ 2,
	    /*6 */ 1,
	    /*7 */ 1,
	    /*8 */ 4,
	    /*9 */ 5,
	    /*10*/ 3,
	    /*11*/ 3,
	    /*12*/ 1,
	    /*13*/ 1,
	    /*14*/ 1,
	    /*15*/ 1
    };
	
	// Table.
	private static final int[][] table =
	{         // 0,  1,  2,  3, 4,  5,  6,  7,    8,  9, 10,    11,  12,13, 14,  15,    16,   17,  18,19,20
		      // (,  ),  {,  }, =,  ;, +-,int,float, if, id,entero,real, $,Fun,Tipo,Bloque,SIntr,Intr, E, T
		/* 0 */{ 0,  0,  0,  0, 0,  0,  0,  2,    3,  0,  0,     0,   0, 0,  1,   4,     0,    0,   0, 0, 0},
		/* 1 */{ 0,  0,  0,  0, 0,  0,  0,  0,    0,  0,  0,     0,   0,99,  0,   0,     0,    0,   0, 0, 0},
		/* 2 */{ 0,  0,  0,  0, 0,  0,  0,  0,    0,  0, -2,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 3 */{ 0,  0,  0,  0, 0,  0,  0,  0,    0,  0, -3,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 4 */{ 0,  0,  0,  0, 0,  0,  0,  0,    0,  0,  5,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 5 */{ 6,  0,  0,  0, 0,  0,  0,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 6 */{ 0,  7,  0,  0, 0,  0,  0,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 7 */{ 0,  0,  9,  0, 0,  0,  0,  0,    0,  0,  0,     0,   0, 0,  0,   0,     8,    0,   0, 0, 0},
		/* 8 */{ 0,  0,  0,  0, 0,  0,  0,  0,    0,  0,  0,     0,   0,-1,  0,   0,     0,    0,   0, 0, 0},
		/* 9 */{ 0,  0,  9,  0, 0,  0,  0,  2,    3, 24, 15,     0,   0, 0,  0,  12,    10,   29,  11, 0, 0},
		/* 10*/{ 0,  0, -7, -7, 0,  0,  0, -7,   -7, -7, -7,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 11*/{ 0,  0, -6, -6, 0,  0,  0, -6,   -6, -6, -6,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 12*/{ 0,  0,  0,  0, 0,  0,  0,  0,    0,  0, 13,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 13*/{ 0,  0,  0,  0, 0, 14,  0,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 14*/{ 0,  0,-10,-10, 0,  0,  0,-10,  -10,-10,-10,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 15*/{ 0,  0,  0,  0,16,  0,  0,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 16*/{ 0,  0,  0,  0, 0,  0,  0,  0,    0,  0, 18,    17,  19, 0,  0,   0,     0,    0,   0,21,20},
		/* 17*/{ 0,-13,  0,  0, 0,-13,-13,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 18*/{ 0,-15,  0,  0, 0,-15,-15,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 19*/{ 0,-14,  0,  0, 0,-14,-14,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 20*/{ 0,-12,  0,  0, 0,-12,-12,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 21*/{ 0,  0,  0,  0, 0, 32, 22,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 22*/{ 0,  0,  0,  0, 0,  0,  0,  0,    0,  0, 18,    17,  19, 0,  0,   0,     0,    0,   0, 0,23},
		/* 23*/{ 0,-11,  0,  0, 0,-11,-11,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 24*/{25,  0,  0,  0, 0,  0,  0,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 25*/{ 0,  0,  0,  0, 0,  0,  0,  0,    0,  0, 18,    17,  19, 0,  0,   0,     0,    0,   0,26,20},
		/* 26*/{ 0, 27,  0,  0, 0,  0, 22,  0,    0,  0,  0,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 27*/{ 0,  0,  9,  0, 0,  0,  0,  2,    3, 24, 15,     0,   0, 0,  0,  12,    10,    0,  28, 0, 0},
		/* 28*/{ 0,  0, -9, -9, 0,  0,  0, -9,   -9, -9, -9,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 29*/{ 0,  0,  9, 31, 0,  0,  0,  2,    3, 24, 15,     0,   0, 0,  0,  12,    10,    0,  30, 0, 0},
		/* 30*/{ 0,  0, -5, -5, 0,  0,  0, -5,   -5, -5, -5,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0},
		/* 31*/{ 0,  0, -4, -4, 0,  0,  0, -4,   -4, -4, -4,     0,   0,-4,  0,   0,     0,    0,   0, 0, 0},
		/* 32*/{ 0,  0, -8, -8, 0,  0,  0, -8,   -8, -8, -8,     0,   0, 0,  0,   0,     0,    0,   0, 0, 0}
	};
	
	// Lexical analyzer (plp1).
  	private AnalizadorLexico lexical;
  	
	// Token (plp1).
	private Token tok;
	
	// Stacks: stack for acctions, rules for rules.
	private Stack<Integer> stack, rules;
	
	// Ctor.
	public AnalizadorSintacticoSLR(AnalizadorLexico al)
	{
		lexical = al;
		stack = new Stack<Integer>();
		rules = new Stack<Integer>();
		stack.push(0); // Add 0 (first state);
		tok = lexical.siguienteToken(); // Read token.
		updateToken(); // Update to plp2.
	}

	// Syntax Error.
	private void syntaxError()
	{
		StringBuilder str;

		if(tok.tipo == Token.EOF - 2)
			str = new StringBuilder("Error sintactico: encontrado fin de fichero, esperaba");
		else
			str = new StringBuilder("Error sintactico (" + tok.fila + "," + tok.columna + "): encontrado '" + tok.lexema + "', esperaba");

		int aux = -1;
		for(int idx = 0; idx < tokensPL2.size(); ++idx) // Look up for own list.
		{
			aux = table[stack.lastElement()][idx]; 
			if(aux != 0)
				str.append(" " + tokensPL2.get(idx)); // Append
		}

		System.err.println(str);
		System.exit(-1);
	}
	
	// Analyze file.
	public void analizar()
	{
		boolean keepAnalyzing = true;
		int newState = -1;
		
		while(keepAnalyzing)
		{
			newState = table[stack.lastElement()][tok.tipo]; // Read state.
			
			if(newState == 0)
				syntaxError();
			else if(newState == 99)
			{
				keepAnalyzing = false;
				break;
			}
			if(newState > 0) // Desplazar.
			{
				stack.push(newState);
				tok = lexical.siguienteToken();
				updateToken(); // Update to plp2.
			}
			else // Reducir.
			{
				rules.push(-newState);
				for(int i = 0; i < rightParts[-newState]; ++i) // Sacamos items de la lista
					stack.pop();
				
				newState = table[stack.lastElement()][leftParts[-newState]];
				stack.push(newState);
			}
		}
		
		for(int i = rules.size() - 1; i >= 0; --i)
		{
			System.out.print(rules.get(i));
			if(i != 0)
				System.out.print(" ");
		}
		
		System.out.print("\n");
	}

}
