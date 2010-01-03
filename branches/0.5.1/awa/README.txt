
INDEX:
0.- Introduction
1.- How to use this files
2.- How to produce this files
3.- Bugs / Updates to this format
4.- Others

0.- INTRODUCTION

This is the award's directory.

Here you can find several award configuration files.

The tpea.awa or was.awa can be used as an example to create another awards. Please send
me your award files and I will add it to this directory to make them
available to the public.

The txt files show incomplete awa files that can be used to create (please email them to me to be offered here) another awa files.


If you have information about other awards, please send me the information.
You have information about how to build this kind of files and create your award in the section 2.

If you find a bug in an award, please report it.


1.- HOW TO USE THESE FILES

Using this award files is really easy. Just copy them into your .klog
directory and add it to KLog from the award's configuration tab.
Now, Klog can only support ONE award per entity.

Every time you enter the call of an entity with an award configured, the
name of the award will be displayed on the Other/Local tab and you will be
able to select the local award reference.




2.- HOW TO PRODUCE THESE FILES

The KLog award file format is inspired in the ADIF format.

Basically an award has to follow some simple rules.
The following is an example based on the TPEA.
==== example ==== example ==== example ==== example ====
<award_entity:2>EA
<award_entity:3>EA6
<award_entity:3>EA8
<award_entity:3>EA9
<award_name:4>TPEA
<award_references:2>2
<award_version:8>20060810
<award_sponsor:3>URE
<award_maintainer:5>EA4TV
<eoh>

<award_refnumber:1>1 <award_subdivision:3>EA1 <award_abbrev:1>O <award_refname:8>Asturias <eor>
<award_refnumber:1>2 <award_subdivision:3>EA1 <award_abbrev:2>AV <award_refname:5>Avila <eor>
==== example ==== example ==== example ==== example ====

MANDATORY: The award files has a header (that ends with the <EOH> tag) where the general information about the award is included.

Fields:
MANDATORY: award_entity: (string) The entity where the award is offered. More than one entity can appear but at least one has to.
MANDATORY: award_name: (string) The name of the award. It has to be a short name. (if long name, it may not be displayed correctly on the GUI).
MANDATORY: award:_references: (int) How many references has this award.
OPTIONAL:  award_version: (int) Gives information about the version number of this award file. The DNS-like (based on dates) format number is recommended but basically a secuential number.
OPTIONAL:  award_sponsor: (string) Who is the sponsor of this award. To be defined if just the name, an URL, ...
OPTIONAL:  award_maintainer: (string) Who is the maintainer/author of this award file.
MANDATORY: eoh: End Of Header.

After the <EOH> the references data are shown.
Each reference data has some mandatory fields and some optional fields. 
Each reference ends with a <EOR> (End Of registry) tag.

MANDATORY: award_refnumber: (int) The unique number of the reference in the award. It is to be defined if can be an OPTIONAL information.
MANDATORY: award_abbrev: (string) The short name of the reference (recommended: 1-3 letters).
MANDATORY: award_refname: (string) The name of the reference. It is to be defined if can be an OPTIONAL information.
OPTIONAL:  award_subdivision (string) The prefix of this special reference inside the award. Examples: a region/province/state inside the same entity.
MANDATORY: eor:  End of registry. The data of this registry ends when this tag is shown.



3.- BUGS / UPDATES TO THIS FORMAT
  1.- Commplete definition of the MANDATORY/OPTIONAL fields.
	The fields that has been defined when writing this doc have been stated as MANDATORY by default.
   


4.- OTHERS

License: This award definition format is under: 
http://creativecommons.org/licenses/by-sa/3.0/

That's all.

Thank you for your support.

Enjoy KLog!

73 de Jaime, EA4TV
