/*
 * HisDevFolderRoot.cpp

 *
 *  Created on: May 24, 2013
 *      Author: root
 */

#include "Helpers/logger.h"
#include "HisDevFolderRoot.h"


HisDevFolderRoot::HisDevFolderRoot(string pfileName)
{
	doc = NULL;
	fileName = pfileName;
	folder = NULL;
}

bool MFindProc(HisBase* hisbase,void* args)
{
	CUUID* uuid = (CUUID*)args;
	HisDevValueId* valueid = dynamic_cast<HisDevValueId*>(hisbase);
	if (valueid!=NULL)
	{
		if (valueid->GetDeviceValueId()==(*uuid)) return true;
	}
	return false;
}

bool FindExpressionProc(HisBase* hisbase,void* args)
{
	CUUID* uuid = (CUUID*)args;
	IExpression* expression = dynamic_cast<IExpression*>(hisbase);
	if (expression!=NULL)
	{
		if (expression->GetRecordId() ==(*uuid)) return true;
	}
	return false;
}

IExpression* HisDevFolderRoot::FindExpression(CUUID expressionId)
{
	if (this->folder!=NULL)
	{
		HisBase* expressionBase = folder->Find(FindExpressionProc,&expressionId);
		return dynamic_cast<IExpression*>(expressionBase);
	}
	return NULL;
}

HisDevValueId* HisDevFolderRoot::FindValueId(CUUID devValueId)
{
	if (this->folder!=NULL)
	{
		HisBase* valueid = folder->Find(MFindProc,&devValueId);
		return dynamic_cast<HisDevValueId*>(valueid);
	}
	return NULL;
}

void HisDevFolderRoot::Load()
{
	xmlNodePtr root_node = NULL;/* node pointers */
	//xmlDtdPtr dtd = NULL;       /* DTD pointer */
	if (doc!=NULL)
	{
		/*free the document */
		xmlFreeDoc(doc);

		 /*
		 *Free the global variables that may
		 *have been allocated by the parser.
		 */
		xmlCleanupParser();

		doc = NULL;
	}

	/*
	 * Creates a new document, a node and set it as a root node
	 */
	doc = xmlReadFile(fileName.c_str(), NULL, 0);
	if (doc == NULL) {
		doc = xmlNewDoc(BAD_CAST "1.0");
	}

	root_node = xmlDocGetRootElement(doc);
	if (root_node==NULL)
	{
		folder = new HisDevFolder("root");
		root_node = folder->GetNodePtr();
		xmlDocSetRootElement(doc,root_node);
		Save();
	}
	else
	{
		folder = new HisDevFolder(root_node);
	}

	folder->Load();
}

void HisDevFolderRoot::Save()
{
	folder->Save();

	int result = xmlSaveFormatFileEnc( fileName.c_str(), doc,"UTF-8", 1);

	if (result<0)
	{
		CLogger::Error("Error while writing folders.xml");
		throw std::domain_error("Error while writing folders.xml");
	}

//	FILE* f = fopen(fileName.c_str(),"w");
//	if (f != NULL)
//	{
//		xmlDocDump( f, doc);
//		fclose( f );
//	}
}

HisDevFolder* HisDevFolderRoot::GetFolder()
{
	if (folder==NULL) Load();
	if (folder==NULL) folder = new HisDevFolder("root");
	return folder;
}


HisDevFolderRoot::~HisDevFolderRoot()
{
	delete(folder);
	/*free the document */
	xmlFreeDoc(doc);

	 /*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();

	doc = NULL;
}
