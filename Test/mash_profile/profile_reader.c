/*
 * profile_reader.c
 *
 *  Created on: 19 dec 2015
 *      Author: carltmik
 */

/**
 * section: xmlReader
 * synopsis: Parse and validate an XML file with an xmlReader
 * purpose: Demonstrate the use of xmlReaderForFile() to parse an XML file
 *          validating the content in the process and activating options
 *          like entities substitution, and DTD attributes defaulting.
 *          (Note that the XMLReader functions require libxml2 version later
 *          than 2.6.)
 * usage: reader2 <valid_xml_filename>
 * test: reader2 test2.xml > reader1.tmp && diff reader1.tmp $(srcdir)/reader1.res
 * author: Daniel Veillard
 * copy: see Copyright for the status of this software.
 */

#include <stdio.h>
#include <libxml/xmlreader.h>



/**
 * processNode:
 * @reader: the xmlReader
 *
 * Dump information about the current node
 */

void
processStep (char *docname)
{

  xmlDocPtr doc;
  xmlNodePtr cur;
  doc = xmlParseFile (docname);
  if (doc == NULL)
    {
      fprintf (stderr, "Document not parsed successfully. \n");
      return;
    }

  cur = xmlDocGetRootElement (doc);
  if (cur == NULL)
    {
      fprintf (stderr, "empty document\n");
      xmlFreeDoc (doc);
      return;
    }

  if (xmlStrcmp (cur->name, (const xmlChar *) "mash_profile"))
    {
      fprintf (stderr, "document of the wrong type, root node != story");
      xmlFreeDoc (doc);
      return;
    }



  cur = cur->xmlChildrenNode;
  while (cur != NULL)
    {
      if ((!xmlStrcmp (cur->name, (const xmlChar *) "step")))
	{
	  parseStory (doc, cur);
	}

      if ((!xmlStrcmp (cur->name, (const xmlChar *) "grainbill")))
	{
	  parseGrainbill(doc, cur);
	}

      cur = cur->next;
    }
  xmlFreeDoc (doc);
  return;
}


void
parseStory (xmlDocPtr doc, xmlNodePtr cur)
{
  xmlChar *key;
  cur = cur->xmlChildrenNode;
  while (cur != NULL)
    {
      if ((!xmlStrcmp (cur->name, (const xmlChar *) "temp")))
	{
	  key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
	  printf ("temp: %s\n", key);
	  xmlFree (key);
	}
      if ((!xmlStrcmp (cur->name, (const xmlChar *) "time")))
	{
	  key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
	  printf ("time: %s\n", key);
	  xmlFree (key);
	}
      if ((!xmlStrcmp (cur->name, (const xmlChar *) "name")))
	{
	  key = xmlNodeListGetString (doc, cur->xmlChildrenNode, 1);
	  printf ("name: %s\n", key);
	  xmlFree (key);
	}


      cur = cur->next;
    }
  return;
}

void
parseGrainbill(xmlDocPtr doc, xmlNodePtr cur)
{
  xmlChar *key;

  if ((!xmlStrcmp (cur->name, (const xmlChar *) "grainbill")))
	{
	  key = xmlNodeListGetString (doc, cur, 1);
	  printf ("grainbill: %s\n", key);
	  xmlFree (key);
	}

}
void
read_profile ()
{

  /*
   * this initialize the library and check potential ABI mismatches
   * between the version it was compiled for and the actual shared
   * library used.
   */
  LIBXML_TEST_VERSION

  processStep ("mash_profile.xml");


  /*
   * Cleanup function for the XML library.
   */
  xmlCleanupParser ();
  /*
   * this is to debug memory for regression tests
   */
  xmlMemoryDump ();

}

