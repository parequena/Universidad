public class Simbolo
{
    public static final int ENTERO = 1, REAL = 2;

    public String nombre;
    public int tipo; // ENTERO o REAL
    public String trad;
    public Boolean id;

    public Simbolo(String nombre, int tipo, String trad, Boolean id)
    {
        this.nombre = nombre;
        this.tipo = tipo;
        this.trad = trad;
        this.id = id;
    }
    
    public Simbolo()
    {
        nombre = "";
        tipo = 0;
        trad = "";
        id = false;
    }
    
    public Boolean same(Simbolo s)
    {
        Boolean n = nombre.equals(s.nombre);
        Boolean t = tipo == s.tipo;
        Boolean tr = trad.equals(s.trad);
        Boolean idb = (id == s.id);
        
        return n && t && tr && idb;
    }
}