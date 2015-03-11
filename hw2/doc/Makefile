FILE=group

MAIN_LATEX=$(FILE).tex
OUT_PDF=$(FILE).pdf

OUT_PS=$(patsubst %.pdf,$(OUT_PDF))
OUT_DVI=$(patsubst %.pdf,$(OUT_PDF))

DEPS=$(wildcard *.tex) $(wildcard figs/*) $(wildcard *.bib)

SHELL=/bin/bash

all: $(OUT_PDF)

.PHONY: all clean public

$(OUT_PDF): $(DEPS)
	rubber -vv -pd $(FILE)
	@grep --color undefined $(FILE).log && notify-send --urgency=critical "WARNING: UNDEFINED REF" || true

clean:
	rubber -v -pd --clean $(FILE)
