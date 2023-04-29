/*
    Project: Minitunes
    Author : Pablo Requena (Twitter: @conPdePABLO)
    GitHub : https://github. com/parequena/
    Date   : March 2020
*/

/*
NOTICE:
    - This is an old Programacion 2 (Grado Ing. Informatica, Universidad de Alicante) practice. 
    - It's made within more avanced knowledge than that subject. 
    - If you copy this solution, for sure you gonna get a 0. 
    - DO NOT COPY. LEARN. 

ANUNCIO:
    - Esta es una antigua practica de Programacion 2 (Grado Ing. Informatica, Universidad de Alicante)
    - Esta realizada con conocimientos mas avanzados que esa asignatura. 
    - Si copias esta practica, lo mas seguro que es te suspendan. 
    - NO COPIES. APRENDE. 
*/

#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <functional>
#include <optional>

constexpr auto k_mainMenu{ R"(--------------------------------
----------- MiniTunes ----------
--------------------------------
1- Add song
2- Edit song
3- Delete song
4- Show collection
5- Manage playlist
6- Import iTunes JSON data
7- Export playlist to XSPF
8- Save data
9- Load data
0- Play playlist
q- Quit
Option:)" };
constexpr auto k_mainMenuError{ "Error: Unknown option" };
constexpr auto k_songSeparator{ " | " };
// ---------------------------------------------
constexpr auto k_songDemandTitle { "Title:" };
constexpr auto k_songDemandArtist{ "Artist:" };
constexpr auto k_songDemandAlbum { "Album:" };
constexpr auto k_songDemandGenre { "Genre:" };
constexpr auto k_songDemandUrl   { "Url:" };
// ---------------------------------------------
constexpr auto k_songAddError1{ "The song" };
constexpr auto k_songAddError2{ "is already in the collection" };
// ---------------------------------------------
constexpr auto k_search   { "Search:" };
constexpr auto k_noResults{ "No results" };
// ---------------------------------------------
constexpr auto k_editSelectSong{ "Select song:" };
constexpr auto k_editError     { "Error: Unknown song "};
constexpr auto k_editOptions   { "Edit (1-Title, 2-Artist, 3-Album, 4-Genre, 5-Url):" };
// ---------------------------------------------
constexpr auto k_deleteConfirm{ "Delete" };
constexpr auto k_deleteConfirm2{ "? (Y/N)" };
constexpr auto k_deleteError{ "Song not deleted" };

template <class T>
void cin(T& val) noexcept { std::cin >> val; clsCin(); }

// Song
struct Song_t
{
	// Id. 
    uint8_t m_id{ 0 };
	
	// Title. 
    std::string m_title{};

	// Artist. 
    std::string m_artist{};

	// Album
    std::string m_album{};

	// Genre. 
    std::string m_genre{};

	// Url. 
    std::string m_url{};

	inline friend std::ostream& operator<<(std::ostream& os, const Song_t& s)
	{
        os  << static_cast<int>(s. m_id) << k_songSeparator << s. m_title << k_songSeparator
            << s. m_artist << k_songSeparator << s. m_album << k_songSeparator
            << s. m_genre;

        return os;
	}

	inline friend bool operator==(const Song_t& lhs, const Song_t& rhs) noexcept
	{
        return lhs.m_title == rhs.m_title
            && lhs.m_artist == rhs.m_artist;
	}

	inline bool findBySubstring(const std::string& str) const noexcept
	{
        return m_title.find(str) != std::string::npos
            || m_artist.find(str) != std::string::npos
            || m_album.find(str) != std::string::npos
            || m_genre.find(str) != std::string::npos;
	}

	std::string& getValue(const int idx)
	{
        if (idx == 1) return m_title;
        if (idx == 2) return m_artist;
        if (idx == 3) return m_album;
        if (idx == 4) return m_genre;

        return m_url;
	}
};

// Collection
struct Collection_t
{
	// Next song id.
    uint8_t m_id{ 1 };
	
	// Song collection.
    std::vector<Song_t> m_songs{};
};

/////////////////////////////////////////////////////////
// First subject (/pdf/prog2p1).
/////////////////////////////////////////////////////////

// Funcion que busca la cancion que se le pasa por parametro en la coleccion. Si ya existe una con el mismo tıtulo y autor, debe devolver
// true, y en caso contrario false
bool isSongInCollection(const Collection_t& collection, const Song_t& song) noexcept
{
	const auto& cs = collection.m_songs;
	
    return std::find(cs.begin(), cs.end(), song) != cs.end();
}

// Funcion que pide por teclado todos los datos de la cancion y la devuelve. 
Song_t demandSong()
{
    Song_t song;

    std::cout << k_songDemandTitle << "\n"; std::getline(std::cin, song.m_title);
    std::cout << k_songDemandArtist << "\n"; std::getline(std::cin, song.m_artist);
    std::cout << k_songDemandAlbum << "\n"; std::getline(std::cin, song.m_album);
    std::cout << k_songDemandGenre << "\n"; std::getline(std::cin, song.m_genre);
    std::cout << k_songDemandUrl << "\n"; std::getline(std::cin, song.m_url);

    return song;
}

// Funcion para añadir una cancion a la coleccion. Si ya existe(para
// ello se debe llamar a la funcion anterior), debe devolver false y no
// añadirla. En caso contrario la cancion debe añadirse a la coleccion
// con el identificador id = idNextSong, e incrementar idNextSong para
// la siguiente cancion que vaya a insertarse en la coleccion. 
// Para añadir un elemento e a un vector v, puedes usar : v. push_back(e); . 
// Esta funcion debe llamar a demandSong para pedir los datos de la cancion. 
bool addSong(Collection_t& collection) noexcept
{
    auto& song = demandSong();
    const auto exists = isSongInCollection(collection, song);
    if (exists)
    {
        std::cerr << k_songAddError1 << song.m_title << k_songAddError2 << "\n";
        return false;
    }

	// Add song to collection.
    song.m_id = collection.m_id;
    ++collection.m_id;

    collection.m_songs.emplace_back(song);
	
    return false;
}

// Funcion para imprimir una cancion con su identificador, tıtulo, autor,
// album y genero. Por ejemplo :
// 35 | Here Comes the Sun | The Beatles | Abbey Road | Rock
void printSong(const Song_t& song) noexcept
{
    std::cout << song << "\n";
}

// Funcion para buscar un identificador de cancion(id) en la coleccion. 
// Devuelve la posicion del vector de canciones(songs) donde se ha encontrado, o - 1 si la cancion no esta en la coleccion. 
int findIdSong(const Collection_t& collection, const int id) noexcept
{
	for(auto i = 0; i < collection.m_id; ++i)
        if (id == collection.m_songs[i].m_id) return i;
	
    return -1;
}

// Esta funcion mostrara por pantalla las canciones que cumplan el criterio de busqueda solicitado al usuario. Si la busqueda no produce
// ningun resultado, debe devolver false, y en caso contrario true
bool showCollection(Collection_t& collection)
{
    std::cout << k_search << "\n";
    std::string response{};
    std::getline(std::cin, response);

	if(response.empty())
	{
        for (const auto& s : collection.m_songs)
            std::cout << s << "\n";
		
        return true;
	}

    bool emptySearch = true;

    for (const auto& s : collection.m_songs)
    {
	    if(s.findBySubstring(response))
	    {
            emptySearch = false;
            std::cout << s << "\n";
	    }
    }

    if (emptySearch) std::cerr << k_noResults << "\n";
	
    return false;
}


std::optional<std::reference_wrapper<Song_t>> getSelectedSong(Collection_t& collection)
{
    showCollection(collection);
    std::cout << k_editSelectSong << "\n";
    std::size_t selectedSong{ 0 };
    cin(selectedSong);

    if (selectedSong >= collection.m_id)
    {
        std::cerr << k_editError << selectedSong << "\n";
        return {};
    }

    return collection.m_songs[selectedSong - 1];
}

struct OptToStr { uint8_t opt; std::string str; };
const OptToStr opts[] =
{
    {1, k_songDemandTitle}, {2, k_songDemandArtist},
    {3, k_songDemandAlbum}, {4, k_songDemandGenre},
    {5, k_songDemandUrl},   {0, ""}
};

// Funcion que muestra las canciones mediante showCollection. Si esta
// devuelve true, a continuacion pedira el numero de cancion a editar,
// que se buscara en la coleccion mediante findIdSong. Si devuelve false
// no se hara nada. Tras localizar la cancion se le pedira al usuario que
// indique el campo a editar y se modificara esta cancion en la coleccion. 
bool editSong(Collection_t& collection) noexcept
{
    const auto songRef = getSelectedSong(collection); // Get selected song.
    if (!songRef) return false;
	
    auto selectedOpt{ 0 };
	do // Ask for option.
	{
        std::cout << k_editOptions << "\n";
        cin(selectedOpt);
        if (selectedOpt > 5) std::cerr << k_mainMenuError << "\n";
    } while (selectedOpt > 5);

    const auto* o = opts;
    while (o->opt) // Check option.
    {
        if (o->opt == selectedOpt)
        {
            std::cout << o->str << "\n";
            break;
        }
        ++o;
    }
	
    auto& value = songRef->get().getValue(selectedOpt);
    std::getline(std::cin, value);	
	
    return true;
}

// Funcion que muestra las canciones mediante showCollection. Si esta
// devuelve true, a continuacion pedira el numero de cancion a borrar,
// que se buscara en la coleccion mediante findIdSong. Si devuelve false
// no se hara nada. Tras localizar la cancion se le pedira al usuario un
// mensaje de confirmacion de borrado, y si su respuesta es afirmativa se
// eliminara la cancion de la coleccion. 
bool deleteSong(Collection_t& collection)
{
    const auto songRef = getSelectedSong(collection);
    if (!songRef) return false;
    std::cout << k_deleteConfirm << " " << songRef->get() << " " << k_deleteConfirm2 << "\n";
    char response{};
    cin(response);

	if(response != 'Y')
	{
        std::cerr << k_deleteError << "\n";
        return false;
	}

    const auto& cs = collection.m_songs;
    const auto sit = std::find(cs.begin(), cs.end(), songRef->get());
    collection.m_songs.erase(sit);
	
    return true;
}

/////////////////////////////////////////////////////////
// Second subject (/pdf/prog2p2).
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
// Third subject (/pdf/prog2p3).
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
/// Aux functions.
/////////////////////////////////////////////////////////
bool exit(Collection_t& collection) noexcept { return false; }

struct MenuOptions_t
{
    char m_response;
    bool (*pf)(Collection_t&);
};

const MenuOptions_t mainMenu[] =
{
    {'1', &addSong},
    {'2', &editSong},
    {'3', &deleteSong},
    {'4', &showCollection},
    {'q', &exit}
};

void cls() noexcept { std::puts("\033[H\033[J"); }
void clsCin() noexcept { while (std::cin.get() != '\n'); }

int main()
{
    char userResp{};
    const MenuOptions_t* menu{ nullptr };
    Collection_t collection{};
    bool printError{ true }, execResult{false};
    cls();
	
    do
    {
        std::cout << k_mainMenu << "\n";
        cin(userResp);
        menu = mainMenu;
        printError = execResult = true;

    	while(menu->m_response != 0)
    	{
    		if(userResp == menu->m_response)
    		{
                printError = false;
                execResult = menu->pf(collection);
                break;
    		}
    		
            ++menu;
    	}

        if(printError) std::cerr << k_mainMenuError << "\n";
    	
    } while (userResp != 'q');
	
    return 0;
}