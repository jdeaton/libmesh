/*!  \mainpage ExodusII API Documentation

\section intro Introduction

EXODUS is the successor of the widely used finite element (FE) data file format EXODUS
(henceforth referred to as EXODUS I) developed by Mills-Curran and Flanagan. It
continues the concept of a common database for multiple application codes (mesh generators,
analysis codes, visualization software, etc.) rather than code-specific utilities, affording
flexibility and robustness for both the application code developer and application code user.
By using the EXODUS data model, a user inherits the flexibility of using a large array of
application codes (including vendor-supplied codes) which access this common data file
directly or via translators.

The uses of the EXODUS data model include the following:
    - Problem definition -- mesh generation, specification of locations of boundary conditions and load application, specification of material types.
    - Simulation -- model input and results output.
    - Visualization -- model verification, results postprocessing, data interrogation, and analysis tracking.

\section avail License and Availability
The EXODUS library is licensed under the BSD open source license.

Copyright (c) 2005 Sandia Corporation. Under the terms of Contract DE-AC04-94AL85000
with Sandia Corporation, the U.S. Government retains certain rights in this software.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice, this list
of conditions and the following disclaimer in the documentation and/or other
materials provided with the distribution.
  -Neither the name of Sandia Corporation nor the names of its contributors may be
used to endorse or promote products derived from this software without specific
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The ExodusII library source code is available on Sourceforge at
http://sourceforge.net/projects/exodusii

For bug reports, documentation errors, and enhancement suggestions, contact:
- Gregory D. Sjaardema
- PHONE: (505) 844-2701
- EMAIL: gdsjaar@sandia.gov

\section devel Development of EXODUS

The evolution of the EXODUS data model has been steered by FE application code developers
who desire the advantages of a common data format. The EXODUS model has been
designed to overcome deficiencies in the EXODUS I file format and meet the following
functional requirements as specified by these developers:
   - Random read/write access.
   - Application programming interface (API) -- provide routines callable from FORTRAN, C, and C++ application codes.
   - Extensible -- allow new data objects to be added without modifying the application programs that use the file format.
   - Machine independent -- data should be independent of the machine which generated it.
   - Real-time access during analysis -- allow access to the data in a file while the file is
being created.

To address these requirements, the open source database library
etCDF (http://www.unidata.ucar.edu/software/netcdf/) was selected to handle the low-level data storage. The EXODUS
II library functions provide the mapping between FE data objects and
netCDF dimensions, attributes, and variables. Thus, the code developer
interacts with the data model using the vocabulary of an FE analyst
(element connectivity, nodal coordinates, etc.) and is relieved of the
details of the data access mechanism. 

Because an EXODUS file is a netCDF file, an application program can
access data via the EXODUS API or the netCDF API directly. Although
accessing the data directly via the netCDF API requires more in-depth
understanding of netCDF, this capability is a powerful feature that
allows the development of auxiliary libraries of special purpose
functions not offered in the standard EXODUS library. For example,
if an application required access to the coordinates of a single node
(the standard library function returns the coordinates for all of the
nodes in the model), a simple function could be written that calls
netCDF routines directly to read the data of interest.

\section descrip Description of Data Objects

The data in EXODUS files can be divided into three primary
categories: initialization data, model, and results.

Initialization data includes sizing parameters (number of nodes,
number of elements, etc.), optional quality assurance information
(names of codes that have operated on the data), and optional
informational text.

The model is described by data which are static (do not change through
time). These data include nodal coordinates, element connectivity
(node lists for each element), element attributes, and node sets and
side sets (used to aid in applying loading conditions and boundary
constraints).

The results are optional and include five types of variables -- nodal,
element, nodeset, sideset, and global -- each of which is stored
through time. Nodal results are output (at each time step) for all the
nodes in the model. An example of a nodal variable is displacement in
the X direction. Element, nodeset, and sideset results are output (at
each time step) for all entities (elements, nodes, sides) in one or
more entity block. For example, stress may be an element
variable. Another use of element variables is to record element status
(a binary flag indicating whether each element is "alive" or "dead")
through time. Global results are output (at each time step) for a
single element or node, or for a single property. Linear momentum of a
structure and the acceleration at a particular point are both examples
of global variables.  Although these examples correspond to typical FE
applications, the data format is flexible enough to accommodate a
spectrum of uses.

A few conventions and limitations must be cited:

 - There are no restrictions on the frequency of results output except
 that the time value associated with each successive time step must
 increase monotonically.
 - To output results at different frequencies (i.e., variable A at
 every simulation time step, variable B at every other time step)
 multiple EXODUS files must be used.
 - There are no limits to the number of each type of results, but once
 declared, the number cannot change.
 - If the mesh geometry or topology changes in time (i.e., number of
 nodes increases, connectivity changes), then the new geometrymust be
 output to a new EXODUS file.

\section int64 Integer Bulkdata Storage Details

The EXODUS database can store integer bulk data, entity map data, and
mesh entity (block/set) ids in either 32-bit or 64-bit integer format. The data
considered "bulk data" are:

 - element, face, and edge connectivity lists,
 - element, face, edge, and node set entity lists,

The entity map data is any data stored in one of the 'map' objects on
the exodus file.  This includes:
 - id maps
 - number maps
 - order maps
 - processor node maps
 - processor element maps.

A mesh entity id is the id of any block (element block, edge block,
...); set (node set, face set, ...), coordinate frame, and
communication map.

When an EXODUS file is created via the ex_create() function, the
'mode' argument provides the mechanism for specifying how integer data
will be passed as arguments to the API functions and also how the
integer data will be stored on the database. The ex_open() function
also provides a mechanism for specifying how integer data will be
passed as arguments. 

The method uses the 'mode' argument to the ex_open() and
ex_create() functions.  The mode is a 32-bit integer in which certain
bits are turned on by or'ing certain predefined constants.  

exoid = ex_create( EX_TEST_FILENAME,
		   EX_CLOBBER|EX_MAPS_INT64_DB|EX_MAPS_INT64_API,
		   &appWordSize, &diskWordSize );

The constants related to the integer size (32-bit or 64-bit)
specification are:

- EX_MAPS_INT64_DB   -- entity map data
- EX_IDS_INT64_DB    -- mesh entity ids
- EX_BULK_INT64_DB   -- bulk data
- EX_ALL_INT64_DB    -- (the above 3 or'd together)
- EX_MAPS_INT64_API  -- entity map data
- EX_IDS_INT64_API   -- mesh entity ids
- EX_BULK_INT64_API  -- bulk data
- EX_ALL_INT64_API   -- (the above 3 or'd together)

The constants that end with "_DB" specify that that particular integer
data is stored on the database as 64-bit integers; the constants that
end with "_API" specify that that particular integer data is passed
to/from API functions as 64-bit integers.  

If the range of the data being transmitted is larger than the
permitted integer range (for example, if the data is stored on the
database as 64-bit ints and the application specifies passing data as
32-bit ints), the api function will return an error.

The three types of integer data whose storage can be specified are
- maps (EX_MAPS_INT64_),
- "bulk data" including connectivity lists and entity lists (EX_BULK_INT64_), and 
- entity ids which are the ids of element, face, edge, and node sets
   and blocks; and map ids (EX_IDS_INT64_)

The function ex_int64_status(exoid) is used to determine the integer
storage types being used for the EXODUS database 'exoid'.  It returns
an integer which can be and'ed with the above flags to determine
either the storage type or function parameter type. 

For example, if
(EX_MAPS_INT64_DB \& ex_int64_status(exoid)) is true, then map data is
being stored as 64-bit integers for that database.

It is not possible to determine the integer data size on a database
without opening the database via an ex_open() call. However, the
integer size specification for API functions can be changed at any
time via the ex_set_int64_status(exoid, mode) function. The mode is
one or more of EX_MAPS_INT64_API, EX_IDS_INT64_API, or
EX_BULK_INT64_API, or'd together.  Any exodus function calls after
that point will use the specified integer size. Note that a call to
ex_set_int64_status(exoid, mode) overrides any previous setting for
the integer sizes used in the API.  The ex_create() function is the
only way to specify the integer sizes specification for database
integers.

\subsection int64_fortran_api Fortran API
The fortran api is uses the same mechanism as was described above for
the C API. If using the "8-byte real and 8-byte int" fortran mode
typically used by the SEACAS applications (the compiler automatically
promotes all integers and reals to 8-byte quantities), then the
fortran exodus library will automatically enable the *_API
options; the client still needs to specify the *_DB options.

\subsection int64_fortran_imp Fortran Implementation

The new capability to pass 64-bit integer data through the fortran and
C API functions simplifies the implementation of the "8-byte real
8-byte int" usage of the exodus library. Previously, the wrapper
routines in addrwrap.F were required to convert the 8-byte integer
data on the client side to/from 4-byte integers on the library
side. This required extra memory allocation and complications that are
now handled at the lowest level in the netcdf library.  The
map-related functions in the fortran api have all been converted to
pass 64-bit integers down to the C API which has removed some code and
simplified those functions.


\section Database Options (Compression, Name Length, File Type)

The ex_set_option() function call is used to set various options on the
database.  Valid values for 'option' are:

|   Option Name          | Option Values
-------------------------|---------------
| EX_OPT_MAX_NAME_LENGTH | Maximum length of names that will be returned/passed via api call.
| EX_OPT_COMPRESSION_TYPE | Not currently used; default is gzip
| EX_OPT_COMPRESSION_LEVEL | In the range [0..9]. A value of 0 indicates no compression
| EX_OPT_COMPRESSION_SHUFFLE | 1 if enabled, 0 if disabled
| EX_OPT_INTEGER_SIZE_API | 4 or 8 indicating byte size of integers used in api functions.
| EX_OPT_INTEGER_SIZE_DB  | Query only, returns 4 or 8 indicating byte size of integers stored on database.

The compression-related options are only available on netcdf-4 files
since the underlying hdf5 compression functionality is used for the
implementation. The compression level indicates how much effort should
be expended in the compression and the computational expense increases
with higher levels; in many cases, a compression level of 1 is
sufficient. 

\defgroup ResultsData Results Data
@{
 This section describes data file utility functions for creating /
 opening a file, initializing a file with global parameters, reading /
 writing information text, inquiring on parameters stored in the data
 file, and error reporting.
@}

\defgroup Utilities Data File Utilities
  @{
This section describes data file utility functions for creating /
opening a file, initializing a file with global parameters, reading /
writing information text, inquiring on parameters stored in the data
file, and error reporting.
  @}

\defgroup ModelDescription Model Description
  @{
The routines in this section read and write information which 
describe an exodus finite element model. This includes nodal 
coordinates, element order map, element connectivity arrays, 
element attributes, node sets, side sets, and object properties.
  @}


*/
