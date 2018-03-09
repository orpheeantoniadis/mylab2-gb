CC = xelatex
FLAGS = -shell-escape
MAIN = report

all: $(MAIN).pdf mostlyclean

$(MAIN).pdf: $(MAIN).tex
	$(CC) $(FLAGS) $(MAIN).tex
	$(CC) $(FLAGS) $(MAIN).tex

mostlyclean:
	rm -rf _minted-$(MAIN) *.log *.aux *.toc

clean:
	rm -rf $(MAIN).pdf _minted-$(MAIN) *.log *.aux *.toc
