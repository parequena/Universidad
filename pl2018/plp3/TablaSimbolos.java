import java.util.ArrayList;

public class TablaSimbolos
{
    public TablaSimbolos padre;
    public ArrayList<Simbolo> simbolos;
    public String prefix;

    public TablaSimbolos(TablaSimbolos padre)
    {
        this.padre = padre;
        simbolos = new ArrayList<Simbolo>();
        prefix = "";
    }

    public TablaSimbolos(TablaSimbolos padre, String prefix)
    {
        this.padre = padre;
        simbolos = new ArrayList<Simbolo>();
        this.prefix = prefix;
    }

    public boolean anyadir(Simbolo s)
    {
        for (Simbolo ss : simbolos)
            if (ss.nombre.equals(s.nombre)) // repetido en el Ã¡mbito
                return false;

        simbolos.add(s);
        return true;
    }

    public String getSymbolName(int idx, Boolean parent)
    {
        String pp = parent ? "" : prefix;

        if (idx >= 0 && idx < simbolos.size()) return pp + simbolos.get(idx).trad;
        return null;
    }
    
    public TablaSimbolos searchTable(String name)
    {
        for (Simbolo s : simbolos)
            if (s.nombre.equals(name)) return this;
        
        if(padre != null)
            return padre.searchTable(name);
        else return null;
    }
    
    public String getSymbolName(String name, Boolean parent)
    {
        Simbolo s = buscar(name);
        String pp = parent ? "" : prefix;
        
        if (s != null) return pp + s.nombre;

        return "";
    }
    
    public String getSymbolTranslation(String name, Boolean parent)
    {
        Simbolo s = buscar(name);
        String pp = parent ? name : prefix + name;
        
        String trad = s.trad.substring(1);
        
        return pp + trad;
    }

    Simbolo buscar(String nombre)
    {
        for (Simbolo s : simbolos)
            if (s.nombre.equals(nombre)) return s;

        if (padre != null)
            return padre.buscar(nombre);
        else return null;
    }
}