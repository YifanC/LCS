#ifndef __HANDLES__H__
#define __HANDLES__H__

/*! \file handles.h
    \brief This header file declares the necessary functions and
    data types for handling integer handles to memory data pointers.
    
    This functionality shall make use of undefined pointers by users
    nearly impossible.
*/

/*! Reserve a new handle for a given memory location. 
 \param handle a pointer to the handle
 \param data the pointer to the memory location to govern with the handle (must not be NULL!)
 \returns EIB7_NoError if a handle could be reserved
*/

EIB7_ERR EIB_ReserveHandle(int *handle, void *data);

/*! Reserve a new slave handle for a given memory location. 
 \param master a pointer to the master handle
 \param handle a pointer to the new handle
 \param data the pointer to the memory location to govern with the handle (must not be NULL!)
 \returns EIB7_NoError if a handle could be reserved
*/

EIB7_ERR EIB_ReserveChildHandle(int master, int *handle, void *data);

/*! Return the data pointer from the handle, if the handle is valid
 \param handle the actual handle, for which the pointer should be retrieved
 \param data a pointer to a pointer to the handle's data
 \returns EIB7_NoError if the handle was valid
*/

EIB7_ERR EIB_RetrieveHandlePtr(int handle, void **data);

/*! Free the data assigned to the handle, and mark the handle as invalid. All slave handles are
  marked invalid, too.
 \param handle the actual handle to be freed
 \returns EIB7_NoError if the handle was valid
*/

EIB7_ERR EIB_FreeHandle(int handle);


#endif /*__HANDLES__H__*/
