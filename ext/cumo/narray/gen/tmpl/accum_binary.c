//<% (is_float ? ["","_nan"] : [""]).each do |nan| %>

<% unless type_name == 'robject' %>
void <%="cumo_#{type_name}_#{name}#{nan}_reduce_kernel_launch"%>(char *p1, char *p2, char *p3, ssize_t s1, ssize_t s2, uint64_t n);
void <%="cumo_#{type_name}_#{name}#{nan}_kernel_launch"%>(char *p1, char *p2, char *p3, ssize_t s1, ssize_t s2, ssize_t s3, uint64_t n);
<% end %>

static void
<%=c_iter%><%=nan%>(cumo_na_loop_t *const lp)
{
    size_t   n;
    char    *p1, *p2, *p3;
    ssize_t  s1, s2, s3;

    CUMO_INIT_COUNTER(lp, n);
    CUMO_INIT_PTR(lp, 0, p1, s1);
    CUMO_INIT_PTR(lp, 1, p2, s2);
    CUMO_INIT_PTR(lp, 2, p3, s3);

    <% if type_name == 'robject' %>
    {
        size_t i;
        CUMO_SHOW_SYNCHRONIZE_FIXME_WARNING_ONCE("<%=name%><%=nan%>", "<%=type_name%>");
        if (s3==0) {
            dtype z;
            // Reduce loop
            CUMO_GET_DATA(p3,dtype,z);
            for (i=0; i<n; i++) {
                dtype x, y;
                CUMO_GET_DATA_STRIDE(p1,s1,dtype,x);
                CUMO_GET_DATA_STRIDE(p2,s2,dtype,y);
                m_<%=name%><%=nan%>(x,y,z);
            }
            CUMO_SET_DATA(p3,dtype,z);
            return;
        } else {
            for (i=0; i<n; i++) {
                dtype x, y, z;
                CUMO_GET_DATA_STRIDE(p1,s1,dtype,x);
                CUMO_GET_DATA_STRIDE(p2,s2,dtype,y);
                CUMO_GET_DATA(p3,dtype,z);
                m_<%=name%><%=nan%>(x,y,z);
                CUMO_SET_DATA_STRIDE(p3,s3,dtype,z);
            }
        }
    }
    <% else %>
    {
        if (s3==0) {
            <%="cumo_#{type_name}_#{name}#{nan}_reduce_kernel_launch"%>(p1,p2,p3,s1,s2,n);
            return;
        } else {
            <%="cumo_#{type_name}_#{name}#{nan}_kernel_launch"%>(p1,p2,p3,s1,s2,s3,n);
        }
    }
    <% end %>
}
//<% end %>

static VALUE
<%=c_func%>_self(int argc, VALUE *argv, VALUE self)
{
    VALUE v, reduce;
    VALUE naryv[2];
    cumo_ndfunc_arg_in_t ain[4] = {{cT,0},{cT,0},{cumo_sym_reduce,0},{cumo_sym_init,0}};
    cumo_ndfunc_arg_out_t aout[1] = {{cT,0}};
    cumo_ndfunc_t ndf = { <%=c_iter%>, CUMO_STRIDE_LOOP_NIP, 4, 1, ain, aout };

    if (argc < 1) {
        rb_raise(rb_eArgError,"wrong number of arguments (%d for >=1)",argc);
    }
    // should fix below: [self.ndim,other.ndim].max or?
    naryv[0] = self;
    naryv[1] = argv[0];
    //<% if is_float %>
    reduce = cumo_na_reduce_dimension(argc-1, argv+1, 2, naryv, &ndf, <%=c_iter%>_nan);
    //<% else %>
    reduce = cumo_na_reduce_dimension(argc-1, argv+1, 2, naryv, &ndf, 0);
    //<% end %>

    v =  cumo_na_ndloop(&ndf, 4, self, argv[0], reduce, m_<%=name%>_init);
    return <%=type_name%>_extract(v);
}

/*
  Binary <%=name%>.

<% if is_float %>
  @overload <%=op_map%>(other, axis:nil, keepdims:false, nan:false)
<% else %>
  @overload <%=op_map%>(other, axis:nil, keepdims:false)
<% end %>
  @param [Cumo::NArray,Numeric] other
  @param [Numeric,Array,Range] axis (keyword) Affected dimensions.
  @param [TrueClass] keepdims (keyword) If true, the reduced axes are left in the result array as dimensions with size one.
<% if is_float %>
  @param [TrueClass] nan (keyword) If true, apply NaN-aware algorithm (avoid NaN if exists).
<% end %>
  @return [Cumo::NArray] <%=name%> of self and other.
*/
static VALUE
<%=c_func(-1)%>(int argc, VALUE *argv, VALUE self)
{
    //<% if !is_object %>
    VALUE klass, v;
    //<% end %>
    if (argc < 1) {
        rb_raise(rb_eArgError,"wrong number of arguments (%d for >=1)",argc);
    }
    //<% if is_object %>
    return <%=c_func%>_self(argc, argv, self);
    //<% else %>
    klass = cumo_na_upcast(rb_obj_class(self),rb_obj_class(argv[0]));
    if (klass==cT) {
        return <%=c_func%>_self(argc, argv, self);
    } else {
        v = rb_funcall(klass, cumo_id_cast, 1, self);
        return rb_funcall2(v, rb_intern("<%=name%>"), argc, argv);
    }
    //<% end %>
}
