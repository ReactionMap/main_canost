// main_canost.h 共通のヘッダーファイル

// 行として読みこむ最大文字数
#define MAX_LINE 256
// 1単語としての最大の大きさ
#define MAX_WORK (MAX_LINE/2)

// 真偽値
#define TRUE 1
#define FALSE 0

// オプションの値
#define CODE 1
#define SERIAL 2
#define FILIATION 3
#define SYMMETRIC 4
#define CONFORM 5
#define CANONICAL 6
#define UNIQUE 7

/* 結合の種類 */
#define SINGLE 1
#define DOUBLE 2
#define TRIPLE 3
#define AROMATIC 4
#define S_OR_D 5
#define S_OR_A 6
#define D_OR_A 7
#define ANY 8 

// CASTコードのUNKNOWN
#define UK (-2)

/* split_component,permutable後にorderingをするか否か */
// しないとき
#define SKIP 0
// するとき
#define TRY 1

/* 順位の付き方 */
// 全て同じ順位
#define TIE 0
// 1番目が異なる
#define FIRST 1
// 3番目が異なる
#define THIRD 3
// 全てが異なる
#define INDIVID 4

/* 順位の付き方 2 */
// 子なし
#define NOCHILD 0
// 0
#define FIRST_ONE 1
// 0-1
#define FIRST_TWO 2
// 0-0
#define TIE_TWO 3
// 0-1-1
#define FIRST_THREE 4
// 0-0-1
#define THIRD_THREE 5
// 0-0-0
#define TIE_THREE 6
// 0-1-2(-3)
#define INDIVID_CHILDREN 7

/* CASTコードを決める軸 */
// 軸なし
#define NOAXIS_TYPE_ONE 0
// TIE_***で軸が未決定
#define NOAXIS_TYPE_TWO 1
// FIRST_ONEで軸が決定
#define AXIS_TYPE_ONE 2
// その他で軸が決定
#define AXIS_TYPE_TWO 3

// 定数としてZERO, MINUSを定義
#define ZERO 0
#define MINUS (-1)

/* state の値 */ 
// 探策済みを表す
#define VISITED 1
// 未探索を表す
#define UNVISITED 0
// 再探索を表す
#define REVISITED 2

/* link の値 */
// 環構造上の原子ではないことを表す
#define BRANCH 0
// 下位に１つの環構造を持つが環構造上の原子ではないことを表す
#define COLUMN 1
// 下位に複数の環構造を持つが環構造上の原子ではないことを表す
#define TRUNK 2
// 独立した１つの環構造上の原子であることを表す
#define LOOP 3
// 環構造上の原子であることを表す(LOOPを除く)
#define CAVE 4
// LOOPである環構造の閉じる原子
#define HITCH 5
// CAVEである環構造の閉じる原子
#define KNOT 6
// permutable でひとつの原子からなる成分になる原子
#define STEM 7

/* couple の値 */
// 環構造を閉じる直前の原子
#define COUPLE 1
// それ以外
#define UNCOUPLE 0

// tupleの大きさ＝最大結合数
#define TUPLE_SIZE 5

// 対称構造を検索する範囲
#define SYM_SEARCH 5

// 結合を表す構造体
struct bond{
  int second; // 結合のもう一方の原子
  int alias; // 仮想的な原子の場合を含めたもう一方の端点
  int btype;  // 結合の種類
  int bstereo;
  int ring; // 環構造の情報
  struct bond *next; // 隣接リストの次の結合を指すポインタ
};

// 原子を表す構造体
struct atom{
  double xco; // x座標
  double yco; // y座標
  double zco; // z座標
  int nbnd; // 結合している原子の個数
  char atm_type[3];  // 原子の種類
  int atmNum; // 原子番号
  char code[5]; // canostコード
  int codeNum; // canostの番号
  struct bond *adj_list; // 隣接リストの先頭の結合を指すポインタ
  struct bond *last; // 隣接リストの最後の結合を指すポインタ
  int state;  // 探策済みか未探索を表す
  int grade;  // 順位
  int layer;  // 属す層の番号
  int link; // 環構造上の原子か否か
  int couple; // 環構造を閉じる直前の原子
  int tuple_parent[TUPLE_SIZE]; // 親に関するtuple
  int tuple_child[TUPLE_SIZE];  // 子に関するtuple
};

// Queue
struct queue{
  int item; // Queueの要素
  struct queue *next; // 次の要素を指すポインタ
};

// 層の先頭を表す構造体
struct level{ 
  int member_Num; // 層中の原子の個数
  int *member; // 層中の原子のリストの先頭の原子を指すポインタ
  struct level *prev; // ひとつ上の層を指すポインタ
  struct level *next; // ひとつ下の層を指すポインタ
};

// CANOST表記用の構造体
struct canost{
  int serial; // シリアル番号
  char cano_code[5]; // canostコード
  struct canost *next; // 次の構造体を指すポインタ
};

// permutable で使う構造体
struct fellow{
  int serial;
  struct fellow *leader;
  struct fellow *next;
};

// permutable で使う構造体
struct amigo{
  int fellows_Num;
  struct fellow *first;
  struct amigo *next;
};

// 関数
/* utile_functions */
char small_to_capital( char c );
int initialize_primary_queue( void );
int add_primary_queue( int x );
int remove_primary_queue( void );
int initialize_second_queue( void );
int add_second_queue( int x );
int remove_second_queue( void );
int initgroup( int n, int *parent, int *treesize );
int findgroup( int k, int *parent );
int joingroup( int p, int q, int *parent, int *treesize );
int join( int p, int q, int *parent, int *treesize );
int comp_int_array( int *intarr1, int length1, int *intarr2, int length2 );

/* 以下それぞれのファイル */
int atomic_num( void );
int recognition( void );
int assign_codeNum( void );
int fun_bfs( void );
int genAtom( int dad, int mum, int stair );
int sort_b_u( void );
int sort_t_d( void );
int update_tuple( int *chain, int length, int upper, int lower );
int lexsort( int *chain, int length, int upper, int lower);
int genlnr_code( void );
int canonicalization( void );
int ordering( void );
int intentional_ordering( void );
int raise_rank( int higher, int *caddy, int space );
int raise_and_order( int higher, int *caddy, int space );
int first_sort( void );
int first_lexsort( int *chain, int length, int lower );
int first_update_tuple( int *chain, int length, int lower );
int second_update_tuple( int *chain, int length, int lower );
int tuple_cmp( int *tuple_right, int *tuple_left );
int bonds_sort( void );
int filiation( void );
int prune_branch( void );
int visit_atom( int visitor, int caller, int *connector, int *linkage, int *visit_order );
int split_component( void );
int sym_substructure( void );
int permutable( void );
int get_dihedral_angle( int alpha, int beta, int gamma, int epsilon );
double calculate_dihedral_angle( int alpha, int beta, int gamma, int epsilon );
int identify_parent( int child );
int identify_ancestors( int parent, int child );
int assign_cast_code( int epsilon );
int ordering_on_cast( void );
int apply_cast( void );
int geometric_order( int *caddy, int scale );
int set_atom( void );
int set_upper_level( void );
int single_child( int scanned );
int linear_pair( int altair, int vega );
int broken_line_pair( int altair, int vega );
int set_children( int *comrade, int comNum, int *standard, int iso_type );
int scan_children( int scanned, int *isolato, int *comrade, int *comNum );
int distinguish_competitive_atoms( int pivot, int *competitor, int compeNum );
int distinguish_competitive_atoms2( int *pivots, int *competitor, int compe_type );
int arrange_ordered_atoms( int *ranker, int rank );
int confine_level( void );
int linearity( int hinge, int parent, int child );
int judge_sym_substruct( void );
int duplicate_levels( void );
int equive_search(char *outfile);
int carbon_code(int carbon);
int oxygen_code(int oxygen);
int halogen_code(int halogen);
int nitrogen_code(int halogen);
