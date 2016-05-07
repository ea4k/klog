BASENAME=klog-handbook
SOURCE=$(BASENAME).xml

.PHONY: all tex html pdf validate publish clean

all: html pdf

html: $(BASENAME).html
$(BASENAME).html: $(SOURCE)
	docbook2html $<

pdf: $(BASENAME).pdf
$(BASENAME).pdf: $(SOURCE)
	docbook2pdf $<

validate:
	xmllint --valid --noout $(SOURCE)

clean:
	rm -f *html *pdf *ps *dvi *tex *~
