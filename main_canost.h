// main_canost.h ���̤Υإå����ե�����

// �ԤȤ����ɤߤ������ʸ����
#define MAX_LINE 256
// 1ñ��Ȥ��Ƥκ�����礭��
#define MAX_WORK (MAX_LINE/2)

// ������
#define TRUE 1
#define FALSE 0

// ���ץ�������
#define CODE 1
#define SERIAL 2
#define FILIATION 3
#define SYMMETRIC 4
#define CONFORM 5
#define CANONICAL 6
#define UNIQUE 7

/* ���μ��� */
#define SINGLE 1
#define DOUBLE 2
#define TRIPLE 3
#define AROMATIC 4
#define S_OR_D 5
#define S_OR_A 6
#define D_OR_A 7
#define ANY 8 

// CAST�����ɤ�UNKNOWN
#define UK (-2)

/* split_component,permutable���ordering�򤹤뤫�ݤ� */
// ���ʤ��Ȥ�
#define SKIP 0
// ����Ȥ�
#define TRY 1

/* ��̤��դ��� */
// ����Ʊ�����
#define TIE 0
// 1���ܤ��ۤʤ�
#define FIRST 1
// 3���ܤ��ۤʤ�
#define THIRD 3
// ���Ƥ��ۤʤ�
#define INDIVID 4

/* ��̤��դ��� 2 */
// �Ҥʤ�
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

/* CAST�����ɤ���뼴 */
// ���ʤ�
#define NOAXIS_TYPE_ONE 0
// TIE_***�Ǽ���̤����
#define NOAXIS_TYPE_TWO 1
// FIRST_ONE�Ǽ�������
#define AXIS_TYPE_ONE 2
// ����¾�Ǽ�������
#define AXIS_TYPE_TWO 3

// ����Ȥ���ZERO, MINUS�����
#define ZERO 0
#define MINUS (-1)

/* state ���� */ 
// õ���Ѥߤ�ɽ��
#define VISITED 1
// ̤õ����ɽ��
#define UNVISITED 0
// ��õ����ɽ��
#define REVISITED 2

/* link ���� */
// �Ĺ�¤��θ��ҤǤϤʤ����Ȥ�ɽ��
#define BRANCH 0
// ���̤ˣ��ĤδĹ�¤����Ĥ��Ĺ�¤��θ��ҤǤϤʤ����Ȥ�ɽ��
#define COLUMN 1
// ���̤�ʣ���δĹ�¤����Ĥ��Ĺ�¤��θ��ҤǤϤʤ����Ȥ�ɽ��
#define TRUNK 2
// ��Ω�������ĤδĹ�¤��θ��ҤǤ��뤳�Ȥ�ɽ��
#define LOOP 3
// �Ĺ�¤��θ��ҤǤ��뤳�Ȥ�ɽ��(LOOP�����)
#define CAVE 4
// LOOP�Ǥ���Ĺ�¤���Ĥ��븶��
#define HITCH 5
// CAVE�Ǥ���Ĺ�¤���Ĥ��븶��
#define KNOT 6
// permutable �ǤҤȤĤθ��Ҥ���ʤ���ʬ�ˤʤ븶��
#define STEM 7

/* couple ���� */
// �Ĺ�¤���Ĥ���ľ���θ���
#define COUPLE 1
// ����ʳ�
#define UNCOUPLE 0

// tuple���礭����������
#define TUPLE_SIZE 5

// �оι�¤�򸡺������ϰ�
#define SYM_SEARCH 5

// ����ɽ����¤��
struct bond{
  int second; // ���Τ⤦�����θ���
  int alias; // ����Ū�ʸ��Ҥξ���ޤ᤿�⤦������ü��
  int btype;  // ���μ���
  int bstereo;
  int ring; // �Ĺ�¤�ξ���
  struct bond *next; // ���ܥꥹ�Ȥμ��η���ؤ��ݥ���
};

// ���Ҥ�ɽ����¤��
struct atom{
  double xco; // x��ɸ
  double yco; // y��ɸ
  double zco; // z��ɸ
  int nbnd; // ��礷�Ƥ��븶�ҤθĿ�
  char atm_type[3];  // ���Ҥμ���
  int atmNum; // �����ֹ�
  char code[5]; // canost������
  int codeNum; // canost���ֹ�
  struct bond *adj_list; // ���ܥꥹ�Ȥ���Ƭ�η���ؤ��ݥ���
  struct bond *last; // ���ܥꥹ�ȤκǸ�η���ؤ��ݥ���
  int state;  // õ���Ѥߤ�̤õ����ɽ��
  int grade;  // ���
  int layer;  // °���ؤ��ֹ�
  int link; // �Ĺ�¤��θ��Ҥ��ݤ�
  int couple; // �Ĺ�¤���Ĥ���ľ���θ���
  int tuple_parent[TUPLE_SIZE]; // �Ƥ˴ؤ���tuple
  int tuple_child[TUPLE_SIZE];  // �Ҥ˴ؤ���tuple
};

// Queue
struct queue{
  int item; // Queue������
  struct queue *next; // �������Ǥ�ؤ��ݥ���
};

// �ؤ���Ƭ��ɽ����¤��
struct level{ 
  int member_Num; // ����θ��ҤθĿ�
  int *member; // ����θ��ҤΥꥹ�Ȥ���Ƭ�θ��Ҥ�ؤ��ݥ���
  struct level *prev; // �ҤȤľ���ؤ�ؤ��ݥ���
  struct level *next; // �ҤȤĲ����ؤ�ؤ��ݥ���
};

// CANOSTɽ���Ѥι�¤��
struct canost{
  int serial; // ���ꥢ���ֹ�
  char cano_code[5]; // canost������
  struct canost *next; // ���ι�¤�Τ�ؤ��ݥ���
};

// permutable �ǻȤ���¤��
struct fellow{
  int serial;
  struct fellow *leader;
  struct fellow *next;
};

// permutable �ǻȤ���¤��
struct amigo{
  int fellows_Num;
  struct fellow *first;
  struct amigo *next;
};

// �ؿ�
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

/* �ʲ����줾��Υե����� */
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
