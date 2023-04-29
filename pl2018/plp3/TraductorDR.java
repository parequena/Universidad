import java.util.ArrayList;
import java.util.NoSuchElementException;
import java.util.StringTokenizer;

public class TraductorDR
{
    private Boolean print;
    private Boolean asig;
    private AnalizadorLexico lexical;
    private Token tok;
    private StringBuilder rules;
    private ArrayList<TablaSimbolos> stackTables = new ArrayList<TablaSimbolos>();
    private ArrayList<String> ifTypes;
    private ArrayList<String> tranOrder = new ArrayList<String>();
    private TablaSimbolos arguments;

    public TraductorDR(AnalizadorLexico al)
    {
        rules = new StringBuilder(); // Init rules.
        print = false; // Debug option.
        lexical = al; // Save the lexical Analyzer.
        tok = lexical.siguienteToken(); // New token.
        arguments = new TablaSimbolos(null); // Symbol arguments.
    }

    private Simbolo searchSymbol(TablaSimbolos ts, String name)
    {
        Simbolo ret = ts.buscar(name);

        if (ret == null)
        {
            for (int i = 0; i < arguments.simbolos.size(); ++i)
                if (arguments.simbolos.get(i).nombre.equals(name)) ret = arguments.simbolos.get(i);
        }

        if (ret == null)
        {
            System.err.println(
                    "Error semantico (" + tok.fila + "," + tok.columna + "): '" + name + "' no ha sido declarado");
            System.exit(-1);
        }

        return ret;
    }

    private String getSymbolName(TablaSimbolos ts, String name)
    {
        TablaSimbolos t = ts.searchTable(name);
        String str = t != null ? t.getSymbolName(name, t.padre == null ? true : false) : "";

        if (str.isEmpty())
        {
            for (int i = 0; i < arguments.simbolos.size(); ++i)
                if (arguments.simbolos.get(i).nombre.equals(name)) str = name;
        }

        if (str.isEmpty())
        {
            System.err.println(
                    "Error semantico (" + tok.fila + "," + tok.columna + "): '" + name + "' no ha sido declarado");
            System.exit(-1);
        }

        return str;
    }

    private String printSymbolType(int t)
    {
        if (t == Simbolo.ENTERO)
            return "integer;";
        else if (t == Simbolo.REAL)
            return "real;";
        else return "";
    }

    private void pair(int toPair)
    {

        if (tok.tipo == toPair)
            tok = lexical.siguienteToken();
        else syntaxError(toPair);

        if (tok.tipo == Token.EOF) System.out.println("");
    }

    private void addRule(int r)
    {
        rules.append(" ").append(r);
    }

    private void syntaxError(int... ord)
    {
        StringBuilder str;

        if (tok.tipo == Token.EOF)
            str = new StringBuilder("Error sintactico: encontrado fin de fichero, esperaba");
        else str = new StringBuilder(
                "Error sintactico (" + tok.fila + "," + tok.columna + "): encontrado '" + tok.lexema + "', esperaba");

        for (int idx = 0; idx < ord.length; ++idx) // Look up for own list.
            str.append(" " + Token.nombreToken.get(ord[idx])); // Append

        System.err.println(str);
        System.exit(-1);
    }

    private String printVariables()
    {
        StringBuilder variables = new StringBuilder();
        Boolean first = true, found = false;
        /*for(int i = 0; i < order.size(); ++i)
        {
            Simbolo or = order.get(i);
            found = false;
            for(int j = 0; j < stackTables.size() && !found; ++j)
            {
                TablaSimbolos t = stackTables.get(j);
                
                for(int k = 0; k < t.simbolos.size() && !found; ++k)
                {                    
                    if(or.same(t.simbolos.get(k)))
                    {
                        if(first)
                        {
                            variables.append("var ");
                            first = false;
                        }
                        else variables.append("    ");
                        variables.append(t.getSymbolName(k, j == 0 ? true : false) + "\n");
                        found = true;
                    }
                }
            }
        }*/
        
        for(int i = 0; i < tranOrder.size(); ++i)
        {
            if(i == 0)
                variables.append("var ");
            else variables.append("    ");
            variables.append(tranOrder.get(i) + "\n");
                
        }
        
        /*for (int i = 0; i < stackTables.size(); ++i) // Iterate on
        {
            for (int j = 0; j < stackTables.get(i).simbolos.size(); ++j)
            {
                if (i == argVaribles && j == 0) // Just at the beginning.
                    variables.append("var ");
                else variables.append("    ");
                variables.append(stackTables.get(i).getSymbolName(j, i == 0 ? true : false) + "\n");
            }
        }*/
        variables.append("\n");
        return variables.toString();
    }

    private String printNewType(String l, String r)
    {
        if (r.isEmpty()) return l;
        if (l.equals("i") && r.equals("i"))
            return "i";
        else if (l.equals("r") && r.equals("r"))
            return "r";
        else if (l.equals("r") && r.equals("i")) return "itor(";

        return "";
    }

    public final String Fun()
    {
        // TablaSimbolos ts = new TablaSimbolos(null);
        // stackTables.add(new TablaSimbolos(null));
        if (tok.tipo == Token.INT || tok.tipo == Token.FLOAT || tok.tipo == Token.EOF)
        {
            addRule(1); // Adding rule to the stack.
            String type = printSymbolType(Tipo());
            String id = tok.lexema;
            pair(Token.ID);
            String args = Args();
            // String bloq = Bloque(null, id);
            Atributos at = Bloque(null, id);

            String variables = printVariables();

            return "function " + id + args + ":" + type + "\n" + variables + at.translation;
        }
        else syntaxError(Token.INT, Token.FLOAT); // Ordered.
        return "";
    }

    public final int Tipo()
    {
        if (tok.tipo == Token.INT)
        {
            addRule(2); // Adding rule to the stack.
            pair(Token.INT);
            return Simbolo.ENTERO;
            // return "integer;";
        }
        else if (tok.tipo == Token.FLOAT)
        {
            addRule(3); // Adding rule to the stack.
            pair(Token.FLOAT);
            return Simbolo.REAL;
            // return "real;";
        }
        else syntaxError(Token.INT, Token.FLOAT); // Ordered.
        return 0;
        // return "";
    }

    public final String Args()
    {
        if (tok.tipo == Token.PARI)
        {
            addRule(4); // Adding rule to the stack.
            pair(Token.PARI);
            String largs = LArgs();
            pair(Token.PARD);
            if (!largs.isEmpty()) // If largs isn't empty, return ( ).
            {
                if (largs.charAt(largs.length() - 1) == ';') // if (a:real;) then
                    largs = largs.substring(0, largs.length() - 1); // (a:real)
                return "(" + largs + ")";
            }
            else return "";
        }
        else syntaxError(Token.PARI); // Ordered.
        return "";
    }

    public final String LArgs()
    {
        if (tok.tipo == Token.INT || tok.tipo == Token.FLOAT)
        {
            addRule(5); // Adding rule to the stack.
            String a = A();
            String masArgs = MasArgs();
            return a + masArgs;
        }
        else if (tok.tipo == Token.PARD)
        {
            // Do stuff -> Epsilon.
            addRule(6); // Adding rule to the stack.
            return "";
        }
        else syntaxError(Token.INT, Token.FLOAT, Token.PARD); // Ordered.
        return "";
    }

    public final String MasArgs()
    {
        if (tok.tipo == Token.COMA)
        {
            addRule(7); // Adding rule to the stack.
            pair(Token.COMA);
            String a = A();
            String masArgs = MasArgs();
            return a + masArgs;
        }
        else if (tok.tipo == Token.PARD)
        {
            // Do stuff -> Epsilon.
            addRule(8); // Adding rule to the stack.
            return "";
        }
        else syntaxError(Token.PARD, Token.COMA); // Ordered.
        return "";
    }

    public final String A() // Adds to the SymbolTable.
    {
        if (tok.tipo == Token.INT || tok.tipo == Token.FLOAT)
        {
            addRule(9); // Adding rule to the stack.
            int t = Tipo();
            String type = printSymbolType(t);
            Atributos at = Var();
            if (!arguments.anyadir(new Simbolo(at.name, t, at.translation, true))) // Add args on symbol list.
            {
                System.err.println("Error semantico (" + tok.fila + "," + tok.columna + "): '" + at.name
                        + "' ya existe en este ambito");
                System.exit(-1);
            }
            // ++argVaribles;
            return at.translation + ":" + type;
        }
        else syntaxError(Token.INT, Token.FLOAT); // Ordered.
        return "";
    }

    public Atributos Var()
    {
        Atributos at = new Atributos();
        if (tok.tipo == Token.AMP)
        {
            addRule(10); // Adding rule to the stack.
            pair(Token.AMP);
            // String id = tok.lexema;
            at.name = tok.lexema;
            at.translation = "var " + tok.lexema;
            pair(Token.ID);
            return at;
        }
        else if (tok.tipo == Token.ID)
        {
            addRule(11); // Adding rule to the stack.
            at.name = at.translation = tok.lexema;
            pair(Token.ID);
            return at;
        }
        else syntaxError(Token.ID, Token.AMP); // Ordered.
        return at;
    }

    public final Atributos Bloque(TablaSimbolos father, String prefix)
    {
        Atributos at = new Atributos();
        // TablaSimbolos ts = new TablaSimbolos(father);
        stackTables.add(new TablaSimbolos(father, prefix));
        if (tok.tipo == Token.LLAVEI)
        {
            addRule(12); // Adding rule to the stack.
            pair(Token.LLAVEI);
            Atributos sinAt = SInstr(stackTables.get(stackTables.size() - 1));
            pair(Token.LLAVED);
            if (sinAt.translation.equals("\n")) sinAt.translation = "doNothing\n";
            at.translation = "begin\n" + sinAt.translation + "end\n";
            at.actualTable = stackTables.get(stackTables.size() - 1);
            return at;
        }
        else syntaxError(Token.LLAVEI); // Ordered.
        return at;
    }

    public final Atributos SInstr(TablaSimbolos ts)
    {
        if (tok.tipo == Token.LLAVEI || tok.tipo == Token.ID || tok.tipo == Token.IF || tok.tipo == Token.INT
                || tok.tipo == Token.FLOAT)
        {
            addRule(13); // Adding rule to the stack.
            // String instr = Instr(ts);
            Atributos at = new Atributos(Instr(ts));
            String sintrp = SInstrp(at.actualTable);
            at.translation += sintrp;
            return at;
        }
        else syntaxError(Token.ID, Token.INT, Token.FLOAT, Token.LLAVEI, Token.IF); // Ordered.
        return null;
    }

    public final String SInstrp(TablaSimbolos ts)
    {
        if (tok.tipo == Token.LLAVEI || tok.tipo == Token.ID || tok.tipo == Token.IF || tok.tipo == Token.INT
                || tok.tipo == Token.FLOAT)
        {
            addRule(14); // Adding rule to the stack.
            // String instr = Instr(ts);
            Atributos at = Instr(ts);
            String sintrp = SInstrp(ts);
            String mid = "\n";
            if (!at.translation.isEmpty() && !sintrp.isEmpty())
            {
                mid = ";\n";
                // at.translation = "doNothing";
            }
            return at.translation + mid + sintrp;
        }
        else if (tok.tipo == Token.LLAVED)
        {
            // Do stuff -> Epsilon.
            addRule(15); // Adding rule to the stack.
            return "";
        }
        else syntaxError(Token.ID, Token.INT, Token.FLOAT, Token.LLAVEI, Token.LLAVED, Token.IF); // Ordered.
        return "";
    }

    private Simbolo takeSymbol(TablaSimbolos ts)
    {
        Simbolo s = new Simbolo();
        s.nombre = s.trad = tok.lexema; // We supose that token isn't an id.
        if (tok.tipo != Token.ID) s.tipo = s.trad.contains(".") ? Simbolo.REAL : Simbolo.ENTERO; // If has a point, is a
                                                                                                 // float.

        if (tok.tipo == Token.ID) s = searchSymbol(ts, s.trad);

        return s;
    }

    private String createItor(String str)
    {
        Boolean end = false;
        StringBuilder ret = new StringBuilder();
        StringTokenizer st = new StringTokenizer(str, " +");
        String strToken = st.nextToken();
        while (strToken.charAt(0) == 'i')
        {
            try
            {
                ret.append("+" + strToken);
                strToken = st.nextToken();
                ret.append(" " + strToken + " ");
                strToken = st.nextToken();
            }
            catch (NoSuchElementException ex)
            {
                end = true;
                break;
            }
        }
        ret.deleteCharAt(ret.length() - 1); // Erase last space
        if (end)
            ret.append(")");
        else ret.append(") +" + strToken);

        if (!end)
        {
            strToken = st.nextToken();
            while (strToken != null)
            {
                try
                {
                    ret.append(" " + strToken);
                    strToken = st.nextToken();
                    ret.append(" +" + strToken);
                    strToken = st.nextToken();
                }
                catch (NoSuchElementException ex)
                {
                    break;
                }
            }
        }

        return ret.toString();
    }

    public final Atributos Instr(TablaSimbolos ts)
    {
        Atributos at = new Atributos();
        if (tok.tipo == Token.LLAVEI)
        {
            addRule(16); // Adding rule to the stack.
            // String bloq = Bloque(ts, ts.prefix + "_");
            at = new Atributos(Bloque(ts, ts.prefix + "_"));
            at.actualTable = ts; // Return the parent table.
            return at;
        }
        else if (tok.tipo == Token.ID)
        {
            asig = true;
            ifTypes = new ArrayList<String>();
            addRule(17); // Adding rule to the stack.
            // String leftAsig = tok.lexema;
            // Simbolo s = searchSymbol(ts, tok.lexema);
            Simbolo s = takeSymbol(ts);
            ifTypes.add(s.tipo == 1 ? "i" : "r"); // Add the symbol type into the typeArray.
            pair(Token.ID);
            Token asig = tok;
            pair(Token.ASIG);
            Simbolo sy = takeSymbol(ts);
            String firstRig = tok.lexema;
            String e = E(ts, 0);
            pair(Token.PYC);

            Boolean b = sameAsPrevious(1);
            if (!b)
            {
                if (ifTypes.get(1).equals("i") && ifTypes.get(2).equals("r"))
                {
                    ifTypes.set(1, "r");
                    firstRig = "itor(" + firstRig + ")";
                    b = true;
                }

                if (ifTypes.get(0).equals("i"))
                {
                    System.err.println("Error semantico (" + asig.fila + "," + asig.columna + "): '" + asig.lexema
                            + "' tipos incompatibles entero/real");
                    System.exit(-1);
                }
            }
            String realType = !b ? printNewType(ifTypes.get(0), ifTypes.get(1)) : firstRig + e;
            if (realType.equals("itor(")) realType += firstRig + " " + createItor(e);
            /*
             * else { at.inherit = realType; realType = s.id ? getSymbolName(ts, s.nombre) :
             * s.nombre; }
             */
            // at.translation = " " + op + at.inherit + " " + realType + a.translation;
            // ts.buscar(leftAsig);
            // TODO: Hacer cambio de tipos

            at.translation = getSymbolName(ts, s.nombre) + " := " + realType;
            at.actualTable = ts;
            return at;
        }
        else if (tok.tipo == Token.IF)
        {
            asig = false;
            ifTypes = new ArrayList<String>();
            addRule(18); // Adding rule to the stack.
            pair(Token.IF);
            pair(Token.PARI);
            Simbolo s = takeSymbol(ts); // Look up for the symbol.
            ifTypes.add(s.tipo == 1 ? "i" : "r"); // Add the symbol type into the typeArray.
            String e = E(ts, 0);
            pair(Token.PARD);
            Atributos inAt = Instr(ts);
            if (inAt.translation.isEmpty()) inAt.translation = "doNothing";
            at.translation = "if " + s.trad + e + " then\n" + inAt.translation; // Always write s.trad.
            at.actualTable = ts;
            return at; // Puede que E aqui, no haga falta.
        }
        else if (tok.tipo == Token.INT || tok.tipo == Token.FLOAT)
        {
            addRule(19); // Adding rule to the stack.
            int t = Tipo();
            String tp = printSymbolType(t);
            String id = tok.lexema;
            pair(Token.ID);
            pair(Token.PYC);
            if (!ts.anyadir(new Simbolo(id, t, id + ":" + tp, true)))// TODO add ts to at
            {
                System.err.println("Error semantico (" + tok.fila + "," + tok.columna + "): '" + at.name
                        + "' ya existe en este ambito");
                System.exit(-1);
            }
            tranOrder.add(ts.getSymbolTranslation(id, ts.padre == null ? true : false));

            at.actualTable = ts;
            // return id + ":" + tp + "\n"; // TODO: Seguramente, no devolverá nada, e irá a
            // la tabla de simbolos.
            return at;
        }
        else syntaxError(Token.ID, Token.INT, Token.FLOAT, Token.LLAVEI, Token.IF); // Ordered
        return at;
    }

    public final String E(TablaSimbolos ts, int index)
    {
        if (tok.tipo == Token.ENTERO || tok.tipo == Token.REAL || tok.tipo == Token.ID)
        {
            addRule(20); // Adding rule to the stack.
            String tk = T(ts);
            Atributos at = Ep(ts, ++index);
            return /* tk + */at.translation;
        }
        else syntaxError(Token.ID, Token.ENTERO, Token.REAL); // Ordered
        return "";
    }

    private Boolean sameAsPrevious(int idx)
    {
        if (asig)
        {
            int pre = idx - 1;
            return ifTypes.get(idx) == ifTypes.get(pre) ? true : false;
        }
        else return false;
    }

    public Atributos Ep(TablaSimbolos ts, int index)
    {
        Atributos at = new Atributos();
        if (tok.tipo == Token.OPAS)
        {
            addRule(21); // Adding rule to the stack.
            String op = tok.lexema;
            pair(Token.OPAS);
            // String id = tok.lexema;
            Simbolo s = takeSymbol(ts);
            String t = T(ts);
            at.inherit = t;
            Atributos a = Ep(ts, ++index);

            Boolean b = sameAsPrevious(index);
            String realType = !b ? printNewType(ifTypes.get(0), t) : t;
            if (realType.equals("itor("))
            {
                realType += s.nombre + ")";
                at.inherit = "r";
            }
            else
            {
                at.inherit = realType;
                realType = s.id ? getSymbolName(ts, s.nombre) : s.nombre;
            }
            at.translation = " " + op + at.inherit + " " + realType + a.translation;
            return at;
        }
        else if (tok.tipo == Token.PYC || tok.tipo == Token.PARD)
        {
            // Do stuff -> Epsilon.
            addRule(22); // Adding rule to the stack.
            --index;
            return at;
        }
        else syntaxError(Token.PARD, Token.PYC, Token.OPAS); // Ordered
        --index;
        return at;
    }

    public final String T(TablaSimbolos ts)
    {
        if (tok.tipo == Token.ENTERO)
        {
            addRule(23); // Adding rule to the stack.
            pair(Token.ENTERO);
            ifTypes.add("i");
            return "i";
        }
        else if (tok.tipo == Token.REAL)
        {
            addRule(24); // Adding rule to the stack.
            pair(Token.REAL);
            ifTypes.add("r");
            return "r";
        }
        else if (tok.tipo == Token.ID)
        {
            addRule(25); // Adding rule to the stack.
            // String id = tok.lexema;
            Simbolo s = takeSymbol(ts);
            String ret = s.tipo == Simbolo.ENTERO ? "i" : "r";
            ifTypes.add(ret);
            pair(Token.ID);
            return ret;
        }
        else syntaxError(Token.ID, Token.ENTERO, Token.REAL);
        return "";
    }

    public void comprobarFinFichero()
    {
        if (tok.tipo != Token.EOF) syntaxError(Token.EOF);
        if (print) System.out.println(rules);
    }
}