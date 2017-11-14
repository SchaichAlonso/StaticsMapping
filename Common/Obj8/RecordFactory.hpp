#pragma once

#include <QMap>
#include <QSharedPointer>

#include "Record.hpp"

namespace Obj8
{
  struct RecordFactory
  {
   ~RecordFactory ();
    
    bool          known (String) const;
    RecordPointer instantiate (StringRef, Parser::LexerContext *) const;
    RecordPointer instantiate (String, StringRef, Parser::LexerContext *) const;
     
    void          addType (Record *);
    
    static RecordFactory *instance ();
    
  protected:
    RecordFactory ();
    typedef QMap<String,RecordPointer> KnownRecords;
    
    static KnownRecords g_known;
  };
}
