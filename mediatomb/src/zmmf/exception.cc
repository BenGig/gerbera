/*  exception.cc - this file is part of MediaTomb.
                                                                                
    Copyright (C) 2005 Gena Batyan <bgeradz@deadlock.dhs.org>,
                       Sergey Bostandzhyan <jin@deadlock.dhs.org>
                                                                                
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
                                                                                
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
                                                                                
    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "exception.h"

#ifndef __CYGWIN__
#include <execinfo.h>
#endif

using namespace zmm;

#define STRACE_TAG "_STRACE_"

Exception::Exception(String message)
{
    this->message = message;
#ifndef __CYGWIN__
    void *b[100];
    int size = backtrace(b, 100);

    stackTrace = Ref<Array<StringBase> >(new Array<StringBase>(size));

    char **s = backtrace_symbols(b, size);
    for(int i = 0; i < size; i++)
    {
        Ref<StringBase> trace(new StringBase(s[i]));
        stackTrace->append(trace);
    }
    free(s);
#endif
}

String Exception::getMessage()
{
    return message;
}

Ref<Array<StringBase> > Exception::getStackTrace()
{
    return stackTrace;
}

void Exception::printStackTrace(FILE *file)
{
    fprintf(file, "Exception: %s\n", message.c_str());
#ifndef __CYGWIN__
    for (int i = 0; i < stackTrace->size(); i++)
    {
        Ref<StringBase> trace = stackTrace->get(i);
        fprintf(file, "%s %i %s\n", STRACE_TAG, i, trace->data);
        fflush(file);
    }
#endif
}

