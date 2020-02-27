This script is intended to parse all the attributes of a gtf file to different columns.

The binary is already available, but if you prefer to compile it yourself, you can do it by using this command

```bash
g++ --std=c++17 gtfParser.cpp -o gtfParser -Wall
```

I did't use a real parser for the command line arguments, so if you don't use it as expected, there could be weird behaviors. You can find the help by running this:

```bash
 ./gtfParser --help
```

here's the output of this command(like I said, it's not intended to be pretty):

This script is intended to parse a gtf file into a tsv format.
You can either use it like this:

	gtfParser gtf_file.gtf

This will give you the parsed output with the base attributes which are listed at the bottom, or you can use your own list of attributes and use it like this:

	gtfParser parse_gtf gtf_file.gtf attributes.txt

The attributes should have the same name in the gtf file and be one per line in the attribute file.
The order will be the one that you define in the attribute file.
If you want to find the list of all attributes in your gtf file, you can find them by using:

	gtfParser find_attributes gtf_file.gtf

Or finally, you can find all attributes in your gtf file and parse it in one motion with:

	gtfParser find_parse gtf_file.gtf

Base attributes:
	gene_id
	gene_name
	gene_biotype
	transcript_id
	transcript_name
	transcript_support_level
	exon_id
	exon_number
	protein_id
	protein_version
