public class Atributos
{
    // Attribute name.
    public String name;

    // Inherit value.
    public String inherit;

    // Translation.
    public String translation;

    // Symbol table.
    public TablaSimbolos actualTable;

    public Atributos()
    {
        name = "";
        inherit = "";
        translation = "";
        actualTable = null;
    };

    public Atributos(String name, String inhe, String trans, TablaSimbolos ts)
    {
        this.name = name;
        inherit = inhe;
        translation = trans;
        actualTable = ts;
    }

    public Atributos(Atributos at)
    {
        name = at.name;
        inherit = at.inherit;
        translation = at.translation;
        actualTable = at.actualTable;
    }
}
