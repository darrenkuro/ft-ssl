# ------------------------ Project Metadata
NAME	:=	ft-ssl
TARGET	:=	ft_ssl

# ------------------------ Directories
SRCDIR	:=	src
OBJDIR	:=	obj
INCDIR	:=	include
LIBDIR	:=	libft

# ------------------------ Files
_SRC	:=	main.c
SRC		:=	$(addprefix $(SRCDIR)/,$(_SRC))
OBJ		:=	$(addprefix $(OBJDIR)/,$(_SRC:.c=.o))

LIBFT_A	:=	libft.a
LIBFT	:=	$(LIBDIR)/$(LIBFT_A)

# ------------------------ Toolchain & Flags
CC			:=	cc
RM			:=	rm -f
CFLAGS		:=	-Wall -Wextra -Werror -MMD -MP
CPPFLAGS	:=	-I $(INCDIR) -I $(LIBDIR)/$(INCDIR)
LDFLAGS		:=	-L $(LIBDIR)
LDLIBS		:=	-lft

# ------------------------ Build Settings
.DEFAULT_GOAL	:=	all

PAD		?=	11
PAD2	:=	10
DEBUG	?=	0
ifeq ($(DEBUG),1)
CFLAGS	+=	-g
endif

# ------------------------ Colors
RESET	:=	\033[0m
RED		:=	\033[31m
GREEN 	:=	\033[32m
GRAY 	:=	\033[90m

define log
printf "%-*s %-*s %s..." $(PAD) "[$(NAME)]" $(PAD2) "$(1)" "$(2)"
endef

define sublog
printf "\n$(GRAY)"; set -o pipefail; $(1) 2>&1 | sed 's/^/  - /' || exit $$$$?; printf "$(RESET)"
endef

define logok
printf " %b\n" "$(GREEN)[OK]$(RESET)"
endef

define logfail
printf " %b\n" "$(RED)[FAIL]$(RESET)"
endef

# ------------------------ Rules & Targets: Submodule
define submodule-target
.PHONY:	$1-init
$1-init:
	@if git submodule status "$1" | grep -Eq '^[-+]'; then \
		$(call log,Initing:,Submodule $1); \
		$(call sublog,git submodule update --init --recursive $1) ; \
		$(call log,Initing:,Submodule $1); \
		$(call logok); \
	fi

.PHONY:	$1-deinit
$1-deinit:
	@if [ -f "$1/.git" ]; then \
		$(call log,Deiniting:,Submodule $1); \
		$(call sublog,git submodule deinit -f $1) ; \
		$(call log,Deiniting:,Submodule $1); \
		$(call logok); \
	fi

.PHONY: $1-update
$1-update:
	@if [ -f "$1/.git" ]; then \
		$(call log,Updating:,Submodule $1); \
		$(call sublog,git submodule update --remote --merge $1) ; \
		$(call log,Updating:,Submodule $1); \
		$(call logok); \
	else \
		$(call log,Updating:,Submodule $1); \
		$(call logfail); \
		printf "  - Try running '$1-init' first.\n"; \
	fi

.PHONY: $1-clean
$1-clean:
	@if [ -f "$1/Makefile" ]; then \
		$(MAKE) -C $1 clean PAD=$(PAD); \
	fi

.PHONY:	$1-fclean
$1-fclean:
	@if [ -f "$1/Makefile" ]; then \
		$(MAKE) -C $1 fclean PAD=$(PAD); \
	fi
endef

$(eval $(call submodule-target,$(LIBDIR)))	# Generate rules for libft


# ------------------------ Rules & Targets
.PHONY:	all
all:	$(TARGET)

.PHONY:	clean
clean:	$(LIBDIR)-clean
	@if [ -d "$(OBJDIR)" ]; then \
		$(call log,Removing:,$(OBJDIR)/); \
		$(RM) -r "$(OBJDIR)"; \
		$(call logok); \
	fi

.PHONY:	fclean
fclean:	clean $(LIBDIR)-fclean $(LIBDIR)-deinit
	@if [ -f "$(TARGET)" ]; then \
		$(call log,Removing:,$(TARGET)); \
		$(RM) "$(TARGET)"; \
		$(call logok); \
	fi

.PHONY:	re
re:	fclean all

$(OBJDIR):
	@$(call log,Creating:,$@/)
	@mkdir -p $@
	@$(call logok)

$(TARGET):	$(LIBFT) $(OBJ)
	@$(call log,Building:,$@)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)
	@$(call logok)

$(OBJDIR)/%.o:	$(SRCDIR)/%.c | $(OBJDIR)
	@$(call log,Compiling:,$(notdir $<))
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	@$(call logok)

$(LIBFT):	| $(LIBDIR)-init
	@$(call log,Building:,$(LIBFT_A))
	@$(MAKE) -C "$(LIBDIR)" --silent PAD=$(PAD)
	@$(call log,Building:,$(LIBFT_A))
	@$(call logok)

.DELETE_ON_ERROR:	# Delete target build that's imcomplete
-include $(OBJ:.o=.d)
