public class AnalizadorSintacticoDR
{
	private boolean print;
	private AnalizadorLexico lexical;
	private Token tok;
	private StringBuilder rules;
	private int order[] = {Token.ID, Token.INT, Token.FLOAT, Token.PARI, Token.PARD, Token.COMA, Token.AMP, Token.LLAVEI, Token.LLAVED, Token.ASIG, Token.PYC, Token.IF, Token.OPAS, Token.ENTERO, Token.REAL};

	public AnalizadorSintacticoDR(AnalizadorLexico al)
	{
		rules = new StringBuilder(); // Init rules.
		print = true; // Debug option.
		lexical = al; // Save the lexical Analyzer.
		tok = lexical.siguienteToken(); // New token.
	}

	private void pair(int toPair)
	{
		if(tok.tipo == toPair)
			tok = lexical.siguienteToken();
		else
			syntaxError(toPair);
	}

	private void addRule(int r)
	{
		rules.append(" ").append(r);
	}

	private void syntaxError(int ... ord)
	{
		StringBuilder str;

		if(tok.tipo == Token.EOF)
			str = new StringBuilder("Error sintactico: encontrado fin de fichero, esperaba");
		else
			str = new StringBuilder("Error sintactico (" + tok.fila + "," + tok.columna + "): encontrado '" + tok.lexema + "', esperaba");

		for(int idx = 0; idx < ord.length; ++idx) // Look up for own list.
			str.append(" " + Token.nombreToken.get(ord[idx])); // Append

		System.err.println(str);
		System.exit(-1);
	}

	public final void Fun()
	{
		if(tok.tipo == Token.INT || tok.tipo == Token.FLOAT || tok.tipo == Token.EOF)
		{
			addRule(1); // Adding rule to the stack.
			Tipo();
			pair(Token.ID);
			Args();
			Bloque();
		}
		else syntaxError(Token.INT, Token.FLOAT); // Ordered.
	}

	public final void Tipo()
	{
		if(tok.tipo == Token.INT)
		{
			addRule(2); // Adding rule to the stack.
			pair(Token.INT);
		}
		else if(tok.tipo == Token.FLOAT) 
		{
			addRule(3); // Adding rule to the stack.
			pair(Token.FLOAT);
		}
		else syntaxError(Token.INT, Token.FLOAT); // Ordered.
	}

	public final void Args()
	{
		if(tok.tipo == Token.PARI)
		{
			addRule(4); // Adding rule to the stack.
			pair(Token.PARI);
			LArgs();
			pair(Token.PARD);
		}
		else syntaxError(Token.PARI); // Ordered.
	}

	public final void LArgs()
	{
		if(tok.tipo == Token.INT || tok.tipo == Token.FLOAT)
		{
			addRule(5); // Adding rule to the stack.
			A();
			MasArgs();
		}
		else if(tok.tipo == Token.PARD)
		{
			// Do stuff -> Epsilon.
			addRule(6); // Adding rule to the stack.
		}
		else syntaxError(Token.INT, Token.FLOAT, Token.PARD); // Ordered.
	}

	public final void MasArgs()
	{
		if(tok.tipo == Token.COMA)
		{
			addRule(7); // Adding rule to the stack.
			pair(Token.COMA);
			A();
			MasArgs();
		}
		else if(tok.tipo == Token.PARD)
		{
			// Do stuff -> Epsilon.
			addRule(8); // Adding rule to the stack.
		}
		else syntaxError(Token.PARD, Token.COMA); // Ordered.
	}

	public final void A()
	{
		if(tok.tipo == Token.INT || tok.tipo == Token.FLOAT)
		{
			addRule(9); // Adding rule to the stack.
			Tipo();
			Var();
		}
		else syntaxError(Token.INT, Token.FLOAT); // Ordered.
	}

	public final void Var()
	{
		if(tok.tipo == Token.AMP)
		{
			addRule(10); // Adding rule to the stack.
			pair(Token.AMP);
			pair(Token.ID);
		}
		else if(tok.tipo == Token.ID)
		{
			addRule(11); // Adding rule to the stack.
			pair(Token.ID);
		}
		else syntaxError(Token.ID, Token.AMP); // Ordered.
	}

	public final void Bloque()
	{
		if(tok.tipo == Token.LLAVEI)
		{
			addRule(12); // Adding rule to the stack.
			pair(Token.LLAVEI);
			SInstr();
			pair(Token.LLAVED);
		}
		else syntaxError(Token.LLAVEI); // Ordered.
	}

	public final void SInstr()
	{
		if(tok.tipo == Token.LLAVEI || tok.tipo == Token.ID || tok.tipo == Token.IF
			|| tok.tipo == Token.INT || tok.tipo == Token.FLOAT)
		{
			addRule(13); // Adding rule to the stack.
			Instr();
			SInstrp();
		}
		else syntaxError(Token.ID, Token.INT, Token.FLOAT, Token.LLAVEI, Token.IF); // Ordered.
	}

	public final void SInstrp()
	{
		if(tok.tipo == Token.LLAVEI || tok.tipo == Token.ID || tok.tipo == Token.IF
			|| tok.tipo == Token.INT || tok.tipo == Token.FLOAT)
		{
			addRule(14); // Adding rule to the stack.
			Instr();
			SInstrp();
		}
		else if(tok.tipo == Token.LLAVED)
		{
			// Do stuff -> Epsilon.
			addRule(15); // Adding rule to the stack.
		}
		else syntaxError(Token.ID, Token.INT, Token.FLOAT, Token.LLAVEI, Token.LLAVED, Token.IF); // Ordered.
	}

	public final void Instr()
	{
		if(tok.tipo == Token.LLAVEI) 
		{
			addRule(16); // Adding rule to the stack.
			Bloque();
		}
		else if(tok.tipo == Token.ID)
		{
			addRule(17); // Adding rule to the stack.
			pair(Token.ID);
			pair(Token.ASIG);
			E();
			pair(Token.PYC);
		}
		else if(tok.tipo == Token.IF)
		{
			addRule(18); // Adding rule to the stack.
			pair(Token.IF);
			pair(Token.PARI);
			E();
			pair(Token.PARD);
			Instr();
		}
		else if(tok.tipo == Token.INT || tok.tipo == Token.FLOAT)
		{
			addRule(19); // Adding rule to the stack.
			Tipo();
			pair(Token.ID);
			pair(Token.PYC);
		}
		else syntaxError(Token.ID, Token.INT, Token.FLOAT, Token.LLAVEI, Token.IF); // Ordered
	}

	public final void E()
	{
		if(tok.tipo == Token.ENTERO || tok.tipo == Token.REAL || tok.tipo == Token.ID)
		{
			addRule(20); // Adding rule to the stack.
			T();
			Ep();
		}
		else syntaxError(Token.ID, Token.ENTERO, Token.REAL); // Ordered
	}

	public final void Ep()
	{
		if(tok.tipo == Token.OPAS)
		{
			addRule(21); // Adding rule to the stack.
			pair(Token.OPAS);
			T();
			Ep();
		}
		else if(tok.tipo == Token.PYC || tok.tipo == Token.PARD)
		{
			// Do stuff -> Epsilon.
			addRule(22); // Adding rule to the stack.
		}
		else syntaxError(Token.PARD, Token.PYC, Token.OPAS); // Ordered
	}

	public final void T()
	{
		if(tok.tipo == Token.ENTERO)
		{
			addRule(23); // Adding rule to the stack.
			pair(Token.ENTERO);
		}
		else if(tok.tipo == Token.REAL)
		{
			addRule(24); // Adding rule to the stack.
			pair(Token.REAL);
		}
		else if(tok.tipo == Token.ID)
		{
			addRule(25); // Adding rule to the stack.
			pair(Token.ID);
		}
		else
		{
			syntaxError(Token.ID, Token.ENTERO, Token.REAL);
		}
	}

	public void comprobarFinFichero()
	{
		if(tok.tipo != Token.EOF)
			syntaxError(Token.EOF);

		if(print)
			System.out.println(rules);
	}
}
