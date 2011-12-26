/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * polarhrm
 * Copyright (C) Thomas Foyth 2011 <thomas.foyth@gmail.com>
 * 
polarhrm is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * polarhrm is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * orginal file by Alfred Mickautsch
 * see libxml2 turorial 
 */

#include "GpxFile.h"
#include <stdio.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#if defined(LIBXML_WRITER_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)

#define MY_ENCODING "UTF-8"


xmlChar *ConvertInput(const char *in, const char *encoding);



void GpxFile::save(Session *session){


	//const char *wptTime = "YYYY-MM-DDThh:mm:ssTZD";
	const char *fix = "2d";
	int sat = 7;

	Gps *gpsWpt = new Gps();
	wDate *startDate;
	wTime *startTime;
	wTime wptTime;

	startDate = session->getStartDate ();
	startTime = startDate->getTime();


	this->WriteHead(this->filename.c_str(), startDate->toTimestamp().c_str());

	
	for (int i=0; i<=session->getNumberOfSamples(); i++) {

		wptTime.setSecond(session->getRecordingInterval()*(i+1));
		wptTime = *startTime+wptTime;
		startDate->setTime(&wptTime);
		gpsWpt = session->samples[i]->getGps();

		this->WriteWayPoint(gpsWpt->getLongitude(),
		                    gpsWpt->getLatitude(),
		                    startDate->toTimestamp().c_str(),
		                    fix,
		                    sat);
	}

	this->WriteFooter();

}




void GpxFile::setPath(string path){
	this->filename = path;
}



void GpxFile::WriteHead(const char *uri, const char *startTime){

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /* Create a new XmlWriter for uri, with no compression. */
    writer = xmlNewTextWriterFilename(uri, 0);
    if (writer == NULL) {
        printf("testXmlwriterFilename: Error creating the xml writer\n");
        return;
    }


    /* Start the document with the xml default for the version,
     * encoding UTF-8 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterStartDocument\n");
        return;
    }

    /* Write a comment as child of EXAMPLE.
     * Please observe, that the input to the xmlTextWriter functions
     * HAS to be in UTF-8, even if the output XML is encoded
     * in iso-8859-1 */
    tmp = ConvertInput("this file was created with libxml2 api\n" \
                       "there is an issue with linebreaks just for convinient display",
                       MY_ENCODING);
    rc = xmlTextWriterWriteComment(writer, tmp);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteComment\n");
        return;
    }
    if (tmp != NULL) xmlFree(tmp);


    /* Start an element named "gpx". Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "gpx");
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Add attributes with name "version" and value "1.0" to gpx. 
	creator="Polar ProTrainer 5 - www.polar.fi"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns="http://www.topografix.com/GPX/1/0"
	xsi:schemaLocation="http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd">
	 */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version",
                                     BAD_CAST "1.0");
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "creator",
                                     BAD_CAST "polarhrm - http://code.google.com/p/polarhrm");
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:xsi",
                                     BAD_CAST "http://www.w3.org/2001/XMLSchema-instance");
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns",
                                     BAD_CAST "http://www.topografix.com/GPX/1/0");
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteAttribute\n");
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xsi:schemaLocation",
                                     BAD_CAST "http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd");
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteAttribute %d\n", __LINE__);
        return;
    }

    /* Write an element named "time" as child of gpx. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "time",
                                         BAD_CAST startTime);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement %d\n", __LINE__);
        return;
    }

    /* Start an element named "trk" as child of gpx. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "trk");
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterStartElement %d\n", __LINE__);
        return;
    }

    /* Start an element named "trkseg" as child of trk. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "trkseg");
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterStartElement %d\n", __LINE__);
        return;
    }
}





void GpxFile::WriteWayPoint(double lon, double lat, const char *wptTime, const char *fix, int sat){

    /* Write an element named "trkpt" as child of trksek. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "trkpt");
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterStartElement %d\n", __LINE__);
        return;
    }

    /* Write lat as atribute */
    rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "lat",
                                     "%.9f", lat );
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatAttribute %d\n", __LINE__);
        return;
    }

    /* Write lon as atribute */
    rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "lon",
                                     "%.9f", lon );
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatAttribute %d\n", __LINE__);
        return;
    }

    /* Write an element named "time" as child of trkpt. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "time",
                                        BAD_CAST wptTime);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteElement %d\n", __LINE__);
        return;
    }

    /* Write an element named "fix" as child of trkpt. 
	 * http://www.topografix.com/gpx/1/1/#type_fixType */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "fix",
                                         BAD_CAST fix);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteElement %d\n", __LINE__);
        return;
    }

    /* Write an element named "sat" as child of trkpt. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "sat","%d", sat);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement %d\n", __LINE__);
        return;
    }

    /* Close the element named trkpt. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterEndElement %d\n", __LINE__);
        return;
    }

}








void GpxFile::WriteFooter(void) {

    /* Close the element named trkseg. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterEndElement %d\n", __LINE__);
        return;
    }

    /* Close the element named trk. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterEndElement %d\n", __LINE__);
        return;
    }

    /* Close the element named gpx. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterFilename: Error at xmlTextWriterEndElement %d\n", __LINE__);
        return;
    }

    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
/*    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return;
    }
*/
    xmlFreeTextWriter(writer);

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();

}




/**
 * ConvertInput:
 * @in: string in a given encoding
 * @encoding: the encoding used
 *
 * Converts @in into UTF-8 for processing with libxml2 APIs
 *
 * Returns the converted UTF-8 string, or NULL in case of error.
 */
xmlChar * GpxFile::ConvertInput(const char *in, const char *encoding){
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) {
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            if (ret < 0) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf
                    ("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else {
        printf("ConvertInput: no mem\n");
    }

    return out;
}





#endif