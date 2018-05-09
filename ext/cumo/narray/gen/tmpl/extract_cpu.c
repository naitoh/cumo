/*
  Extract an element only if self is a dimensionless NArray.
  @overload extract_cpu
  @return [Numeric,Cumo::NArray]
  --- Extract element value as Ruby Object if self is a dimensionless NArray,
  otherwise returns self.
*/
static VALUE
<%=c_func(0)%>(VALUE self)
{
    volatile VALUE v;
    char *ptr;
    narray_t *na;
    GetNArray(self,na);

    if (na->ndim==0) {
        ptr = na_get_pointer_for_read(self) + na_get_offset(self);
        SHOW_SYNCHRONIZE_WARNING_ONCE("<%=name%>", "<%=type_name%>");
        cumo_cuda_runtime_check_status(cudaDeviceSynchronize());
        v = m_extract(ptr);
        na_release_lock(self);
        return v;
    }
    return self;
}