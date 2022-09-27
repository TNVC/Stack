CC   := g++
NAME := a.out
ARGS :=

LOGFILE := compileLog

CFLAGS := -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE #-fsanitize=address,leak #,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
LFLAGS := -lpthread

SRCDIR := src
OBJDIR := objects
INCDIR := include
DEPDIR := dependences

SOURCES 		:= $(wildcard $(addsuffix /*.cpp, $(SRCDIR)) )
OBJECTS     := $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SOURCES)) )
DEPENDENCES := $(patsubst %.cpp, $(DEPDIR)/%.d, $(notdir $(SOURCES)) )

VPATH := $(SRCDIR)

.PHONY: clean run  dependences cleanDependences makeDependencesDir

$(NAME): dependences $(OBJECTS) cleanDependences
	@$(CC) $(LFLAGS) $(OBJECTS) -o $@ 2>$(LOGFILE)

clean:
	@rm -rf $(OBJECTS) $(DEPDIR) $(NAME)

run: clean $(NAME)
	@./$(NAME) $(ARGS)

dependences: makeDependencesDir $(DEPENDENCES)

makeDependencesDir:
	@mkdir -p $(DEPDIR)

$(DEPDIR)/%.d: %.cpp
	@$(CC) -M $(addprefix -I, $(INCDIR)) $< -o $@ 2>$(LOGFILE)

cleanDependences:
	@rm -rf $(DEPDIR)

$(OBJDIR)/%.o: %.cpp
	@$(CC) -c $(addprefix -I, $(INCDIR)) $(CFLAGS) $< -o $@ 2>$(LOGFILE)

include $(wildcard $(DEPDIR)/*.d)
