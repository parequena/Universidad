import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;

public class AnalizadorLexico
{
    private static boolean[] 
    //             0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,   12,  13,   14,   15,  16,  17,   18,   19,  20 
    isFinal = {false,true,true,true,true,true,true,true,true,true,false,true,false,true,false,false,true,true,false,false,false};
    
    private RandomAccessFile fichero; // File.
    private static String letters = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM"; // To check if is a letter.1
    private static String numbers = "1234567890"; // To check if is a number.
    private int row, column, oriRow, oriCol; // Controls row and column, and the original ones;
    private String buffer; // If we need more than 1 character.
	
    ArrayList<String> file = new ArrayList<String>(); // File on ArrayList (shabby, shabby).
	
    // Ctor.
    public AnalizadorLexico(RandomAccessFile entrada)
    {
	buffer = new String();
	fichero = entrada;
	row = 1;
	oriRow = 1;
	oriCol = 1; // First token on (1,1).
	column = 0; // Start on 0, for the first read.
	    
	try
	{
	    readFile();
	}
	catch (Exception e)
	{
	    // Shouldn't fail.
	}
    }
	
    // Converts the file on an ArrayList<String>
    private void readFile() throws IOException
    {
	String line = fichero.readLine();
	    
	while(line != null)
	{
	    file.add(line);
	    line = "";
	    line = fichero.readLine();
	}
    }
	
    // Reads a character from the "file" and controls the row and column;
    private char read()
    {
	char ret;
	++column; // Update column everytime we read.
	
	int aRow = row - 1, aCol = column - 1; // Auxiliars.
    	    
	if(row > /*FMS*/ file.size()) // If we are at the end of the file.
	    ret = (char) Token.EOF; // Static constant of the class.
	else // != eof()
	{
	    if(aCol >= file.get(aRow).length())
	    {
		column = 1; // First column.
		aCol = 0;   // Update auxiliar.
		do
		{
		    ++row; 	// Update row.
		    ++aRow; // Update auxliar.
		    
//		    System.out.println("aRow="+aRow);
		}while(aRow<file.size() /*FMS*/ && file.get(aRow).length() == 0);
	    }
	    
	    if (aRow < file.size()) /*FMS*/
  	    	ret = file.get(aRow).charAt(aCol);
	    else
	    	ret = (char) Token.EOF; // Static constant of the class.
	    
//	    if (aRow < file.size())	
//	    	System.out.println("ret="+ret+", ret(int)="+(int)ret+", aRow="+aRow+", aCol="+aCol+", linea=("+file.get(aRow)+")");
	    
	}
    	    
	return ret;
    }

    // Go backwards on the file.
    private void backwards(int state)
    {
	int back = 0;
    
	if(state == 11 || state == 13 || state == 16) back = 1; // 11, 13, 16 -> *
	else if (state == 17) back = 2;			    // 17 -> **
	else back = 0;
	   
	column = column - back; // Backwards "back" steps.
	if(column < 0) // If we are on negative.
	{
	    --row; // Look up.
	    column = file.get(row).length() - back; // Column = end - back.
	}
	
	buffer = buffer.substring(0, buffer.length() - back);
    }
	
    // Returns the Token looking with the state.
    private Token associatedToken(int estado)
    {
	int state = stateToType(estado); // Convert into Token state.
	    
	if(state == Token.ID) // If we are on ID state.
	{
	    // Check for reserved strings.
	    if(buffer.equals("float")) state = Token.FLOAT;
	    else if(buffer.equals("int")) state = Token.INT;
	    else if(buffer.equals("if")) state = Token.IF;
	}
	
	Token tok = new Token(oriRow, oriCol, buffer, state); // Creating the token.
	buffer = new String(); // Cleaning buffer.
	return tok;
    }
	
    // Prints an error.
    private void LexicalError(char c)
    {
    	if(c == '')
    		System.err.println("Error lexico: fin de fichero inesperado");
    	else
			System.err.println("Error lexico (" + row + "," + column + "): caracter ’" + c + "’ incorrecto");

		System.exit(-1);
    }
	
    // Converts the state into a Token.Type
    private int stateToType(int state)
    {
	// Token
	// 0, 1, 2, 3, 4, 5, 6, 7,  8,   9,    10, 11, 12,   13,   14, 15
	// (, ), ,, &, {, }, =, ;, +-, int, float, if, id, numE, numR, EOF
	// 1, 2, 3, 4, 5, 6, 7, 8,  9,  11,    11, 11, 11,   13,   16, EOF
	// Pablo
	
	if(state == 11) return 12; // 11 id (12)
	else if(state == 13 || state == 17) return 13; // 13 entero* // 17 entero**
	else if(state == 16) return 14; // 16 real*
	else return state - 1; // 1 pari (0) // 2 pard (1) // 3 coma (2) // 4 amp (3) // 5 llavei (4) // 6 llaved (5) // 7 asig (6) // 8 pyc (7) // 9 opas (8) 
    }
	
    // Delta function
    private int delta(int estado, char c)
    {
	switch (estado)
	{
	case 0:
	    if(c == '(') return 1;
	    else if(c == ')') return 2;
	    else if(c == ',') return 3;
	    else if(c == '&') return 4;
	    else if(c == '{') return 5;
	    else if(c == '}') return 6;
	    else if(c == '=') return 7;
	    else if(c == ';') return 8;
	    else if(c == '-' || c == '+') return 9; 
	    else if(letters.indexOf(c) != -1) return 10; // If 'c' is on letters.
	    else if(numbers.indexOf(c) != -1) return 12; // If 'c' is on numbers.
	    else if (c == '/') return 18; // If we got '/'
	    else return -1;
				
	case 10:
	    if(letters.indexOf(c) != -1 || numbers.indexOf(c) != -1) return 10;
	    else return 11;
				
	case 12:
	    if(c == '.') return 14;
	    else if(numbers.indexOf(c) != -1) return 12;
	    else return 13;
				
	case 14:
	    if(numbers.indexOf(c) != -1) return 15;
	    else return 17;
	    
	case 15:
	    if(numbers.indexOf(c) != -1) return 15;
	    else return 16;
				
	case 18: // Searching '/*'
	    if(c == '*') return 19;
	    else return -1;
				
	case 19:
	    if(c == '') LexicalError(''); // Fin de fichero inesperado.
	    if(c == '*') return 20; // Iterate until reach '*'
	    else return 19;
				
	case 20:
	    if (c == '/') return 0; // If we found '*/'
	    else return 19;
				
	default:
	    return -1;
	}
    }

    // Returns a Token.
    public Token siguienteToken()
    {
	int state = 0, newState; // First state = 0, nuevo must be updated.
	// char c = read();
	buffer += read();
	oriRow = row; // Before reading, because row = 1;
	oriCol = column; // After reading, because column = 0;
	
	if(buffer.charAt(0) == '') return new Token(oriRow, oriCol, buffer, Token.EOF); // Creating the EOF token.
	
	while(Character.isWhitespace(buffer.charAt(0))) // if buffer[0] == ' '
	{
	    buffer = new String();
	    buffer += read();
	    oriRow = row; // Before reading, because row = 1;
	    oriCol = column; // After reading, because column = 0;
	}
	    
	do
	{
	    newState = delta(state, buffer.charAt(buffer.length() - 1));
	    if (newState == 0)
	    {
		buffer = new String(); // If we came from a comment, clean the buffer
		oriRow = row;
		oriCol = column + 1;
	    }
	    if (newState == -1) LexicalError(buffer.charAt(buffer.length() - 1));
	    if (isFinal[newState]) // If an state is final1
	    {
		backwards(newState);
		return associatedToken(newState);
	    }
	    else
	    {
		if(newState == 0)
		    System.out.print("");
		state = newState;
		buffer += read();
		if(buffer.charAt(0) == '') return new Token(oriRow, oriCol, buffer, Token.EOF); // Creating the EOF token.
		while(Character.isWhitespace(buffer.charAt(0))) // if buffer[0] == ' '
		{
		    buffer = new String();
		    buffer += read();
		    oriRow = row; // Before reading, because row = 1;
		    oriCol = column; // After reading, because column = 0;
		}
	    }
	} while (true);
    }
}
