CC = pdflatex
MAIN = presentation

all: $(MAIN).pdf mostlyclean

$(MAIN).pdf: $(MAIN).tex
	$(CC) $(MAIN).tex
	$(CC) $(MAIN).tex

mostlyclean:
	rm -rf *.log *.aux *.toc *.bbl *.blg *.out *.lof *.nav *.snm

clean:
	rm -rf $(MAIN).pdf *.log *.aux *.toc *.bbl *.blg *.out *.lof *.nav *.snm
