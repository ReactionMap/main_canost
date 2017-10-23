#Makefile(main_canost) 2010/03/29

PACKAGE	= main_canost

SRC10	= atomic_num.c carbon_code.c oxygen_code.c halogen_code.c nitrogen_code.c recognition.c assign_codeNum.c

SRC11	= fun_bfs.c genAtom.c

SRC12	= confine_level.c prune_branch.c

SRC13	= canonicalization.c

SRC14	= first_sort.c first_update_tuple.c first_lexsort.c second_update_tuple.c tuple_cmp.c

SRC15	= ordering.c

SRC16	= sort_b_u.c sort_t_d.c update_tuple.c lexsort.c

SRC17	= intentional_ordering.c raise_rank.c raise_and_order.c

SRC18	= sym_substructure.c split_component.c permutable.c judge_sym_substruct.c duplicate_levels.c equive_search.c

SRC19	= genlnr_code.c bonds_sort.c filiation.c

SRC20	= ordering_on_cast.c apply_cast.c geometric_order.c

SRC21	= set_atom.c set_upper_level.c set_children.c scan_children.c broken_line_pair.c linear_pair.c single_child.c distinguish_competitive_atoms.c distinguish_competitive_atoms2.c arrange_ordered_atoms.c

SRC22	= get_dihedral_angle.c calculate_dihedral_angle.c identify_parent.c identify_ancestors.c assign_cast_code.c linearity.c

UTILE	= utile_functions.c

SRC1	= $(SRC20) $(SRC21) $(SRC22)

SRC2	= $(SRC15) $(SRC16)

SRC3	= $(SRC13) $(SRC14) $(SRC2) $(SRC17) $(SRC18) $(SRC19)

SRC4	= $(SRC10) $(SRC11) $(SRC12)

SRCS	= $(PACKAGE).c $(SRC1) $(SRC3) $(SRC4) $(UTILE)

HEADS	= $(PACKAGE).h
OBJS	= $(SRCS:.c=.o)

FILES	= README README.ja CHANGE_LOG Makefile code.prior code.prior.org 20-Hydroxyecdysone.mol cubic.mol doc_sym_search.pdf doc_depend_conformation.pdf doc_ordering_rule.pdf $(HEADS) $(SRCS)
VER	= 2.92

### command and flags ###
# uncomment when debugging
#DEBUG	= -ggdb -pg # -lefence

# common (*.o)
LD	= gcc
LDFLAGS	= -g $(DEBUG)
LDLIBS	= -lm

### compile for windows ###
#LDFLAGS	= -mwindows -mno-cygwin $(DEBUG)
#LDFLAGS	= -mwindows -mno-cygwin -Wl,--subsystem,console $(DEBUG)

# C (*.c)
CC	= gcc
CFLAGS	= -g -O2 -Wall $(DEBUG)
CPPFLAGS= -I.

# C++ (*.cc)
CXX	= g++
CXXFLAGS= -g -O2 -Wall $(DEBUG)

# Fortran77 (*.f)
FC	= f77
FFLAGS	= -Wall $(DEBUG)

# Pascal (*.p)
PC	= pc
PFLAGS	= -Wall $(DEBUG)

# etc
SHELL	= /bin/sh
RM	= rm -f
PROF	= gprof


### rules ###

.SUFFIXES:
.SUFFIXES: .o .c .cc .f .p

all: $(PACKAGE)

$(PACKAGE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(OBJS): $(HEADS) Makefile

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
.cc.o:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
.f.o:
	$(FC) $(FFLAGS) -c $< -o $@
.p.o:
	$(PC) $(PFLAGS) $(CPPFLAGS) -c $< -o $@


### useful commands ###

clean:
	$(RM) $(PACKAGE) $(OBJS)
	$(RM) core.* gmon.out *~ dummy_*.dat #*#

tar:
	@echo $(PACKAGE)-$(VER) > .package
	@$(RM) -r `cat .package`
	@mkdir `cat .package`
	@ln $(FILES) `cat .package`
	tar cvf - `cat .package` | gzip -9 > `cat .package`.tar.gz
	@$(RM) -r `cat .package` .package

zip:
	zip -9 $(PACKAGE)-$(VER).zip $(FILES)


prof: run
	$(PROF) $(PACKAGE) | less

run: all
	./$(PACKAGE)  -c f 20-Hydroxyecdysone.mol 20-Hydroxyecdysone.out
