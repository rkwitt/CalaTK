#include "JSONParameterUtils.h"
#include <iomanip>

namespace CALATK
{

CJSONConfiguration::CJSONConfiguration()
  : m_Indent( 2 ), m_IsMasterNode( false ), m_PrintSettings( true ), m_ptrRoot( NULL )
{
}

CJSONConfiguration::~CJSONConfiguration()
{
  if ( m_IsMasterNode )
    {
    // this is the master, need to release the memory
    if ( m_ptrRoot != NULL )
      {
      delete m_ptrRoot;
      m_ptrRoot = NULL;
      }
    }
}

void CJSONConfiguration::PrintSettingsOn()
{
  m_PrintSettings = true;
}

void CJSONConfiguration::PrintSettingsOff()
{
  m_PrintSettings = false;
}

bool CJSONConfiguration::GetPrintSettings()
{
  return m_PrintSettings;
}

void CJSONConfiguration::SetIndent( unsigned int uiIndent )
{
  m_Indent = uiIndent;
}

unsigned int CJSONConfiguration::GetIndent()
{
  return m_Indent;
}

void CJSONConfiguration::SetRootReference( Json::Value& vRoot )
{
  if ( m_ptrRoot != NULL )
    {
    if ( m_IsMasterNode )
      {
      std::cerr << "Trying to overwrite the root node. FORBIDDEN." << std::endl;
      }
    else
      {
      // we can just overwrite it, because everything is ruled by the master root node
      m_ptrRoot = &vRoot;
      }
    }
  else
    {
    m_ptrRoot = &vRoot;
    m_IsMasterNode = false;
    }
}

bool CJSONConfiguration::WriteCurrentConfigurationToJSONFile( std::string sFileName )
{
  std::ofstream outFile;
  outFile.open( sFileName.c_str() );

  if ( !outFile )
    {
    std::cerr << "Could not open " << sFileName << " for writing." << std::endl;
    return false;
    }

  if ( m_ptrRoot != NULL )
    {
    outFile << *m_ptrRoot;
    }

  outFile.close();

  return true;
}

Json::Value* CJSONConfiguration::GetRootPointer()
{
  return m_ptrRoot;
}

void CJSONConfiguration::InitializeEmptyRoot()
{
  if ( m_ptrRoot == NULL )
    {
    m_ptrRoot = new Json::Value( Json::nullValue );
    m_IsMasterNode = true;
    }
  else
    {
    std::cerr << "Error initializing root node. Is already initialized." << std::endl;
    }
}

Json::Value& CJSONConfiguration::GetFromKey( std::string sKey, Json::Value vDefault )
{
  if ( m_ptrRoot == NULL )
    {
    std::cerr << "ERROR: root needs to be initialized before it can be accessed." << std::endl;
    std::cerr << "WARNING: Initializing empty root." << std::endl;
    InitializeEmptyRoot();
    }
  return GetFromKey( *m_ptrRoot, sKey, vDefault, false );
}

Json::Value& CJSONConfiguration::GetFromIndex( Json::Value& vSubTree, Json::ArrayIndex ind, bool bUseIndent )
{
  unsigned int uiIndent;
  if ( bUseIndent )
    {
    uiIndent = m_Indent;
    }
  else
    {
    uiIndent = 0;
    }

  // check if we have such an index, if not, create one
  if ( vSubTree.get( ind, Json::nullValue ) == Json::nullValue )
    {
    // could not find it, create it
    vSubTree[ ind ] = Json::nullValue;
    if ( m_PrintSettings )
      {
      std::cout << std::string( uiIndent, '-' ) << "ind = \"" << ind << "\" : created default tree entry." << std::endl;
      }
    }
  return vSubTree[ ind ];
}

Json::Value& CJSONConfiguration::GetFromKey( Json::Value& vSubTree, std::string sKey, Json::Value vDefault, bool bUseIndent )
{
  unsigned int uiIndent;
  if ( bUseIndent )
    {
    uiIndent = m_Indent;
    }
  else
    {
    uiIndent = 0;
    }

  // check if it is a member, if not, make it one
  if ( !vSubTree.isMember( sKey ) )
    {
    vSubTree[ sKey ] = vDefault;
    if ( m_PrintSettings )
      {
      if ( vDefault.type() != Json::arrayValue && vDefault.type() != Json::objectValue )
        {
        std::cout << std::string( uiIndent, '-' ) << "Key = \"" << sKey << "\" : used default value = " << vDefault;
        }
      else
        {
        std::cout << std::string( uiIndent, '-' ) << "Key = \"" << sKey << "\" : created default value." << std::endl;
        }
      }
    }
  else
    {
    if ( m_PrintSettings )
      {
      if ( vSubTree[ sKey ].type() != Json::arrayValue && vSubTree[ sKey ].type() != Json::objectValue )
        {
        std::cout << std::string( uiIndent, '-' ) << "Key = \"" << sKey << "\" : used JSON-specified value = " << vSubTree.get( sKey, vDefault );
        }
      else
        {
        std::cout << std::string( uiIndent, '-' ) << "Key = \"" << sKey << "\" : used JSON value." << std::endl;
        }
      }
    }
  return vSubTree[ sKey ];
}

std::string CJSONConfiguration::ReadFileContentIntoString( std::string sFileName )
{
  std::string outputString ="";
  std::ifstream inputFile;
  inputFile.open( sFileName.c_str() );

  if ( inputFile.is_open() )
    {
    // could successfully be opened, now read it
    std::string sLine;
    while ( inputFile.good() )
      {
      std::getline( inputFile, sLine );
      outputString += sLine + "\n";
      }
    inputFile.close();

    }
  else
    {
    throw std::runtime_error( "Could not open file " + sFileName );
    outputString = "";
    }

  return outputString;
}

bool CJSONConfiguration::ReadJSONFile( std::string sFileName )
{
  if ( m_ptrRoot != NULL )
    {
    if ( m_IsMasterNode )
      {
      // delete 
      delete m_ptrRoot;
      m_ptrRoot = NULL;
      }
    else
      {
      std::cerr << "Trying to read file into non-master node. FORBIDDEN." << std::endl;
      return false;
      }
    }

  m_ptrRoot = new Json::Value;

  std::cout << "Parsing input file " << sFileName << " ... ";
    
  Json::Reader reader;
  std::string config_doc = ReadFileContentIntoString( sFileName );
  bool parsingSuccessful = reader.parse( config_doc, *m_ptrRoot );
  
  if ( !parsingSuccessful )
    {
    std::cout << "failed." << std::endl;

    // report to the user the failure and their locations in the document.
    std::cout  << "Failed to parse configuration\n"
               << reader.getFormattedErrorMessages();

    return false;
    }
  std::cout << "succeeded." << std::endl;

  return true;
}

} // end namespace