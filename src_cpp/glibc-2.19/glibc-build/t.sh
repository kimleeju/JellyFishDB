
st=$LFS_TGT \

--build=$(../scripts/config.guess) \

--disable-profile \

--enable-kernel=2.6.32 \

--with-headers=/tools/include \

libc_cv_forced_unwind=yes \

libc_cv_ctors_header=yes \

libc_cv_c_cleanup=yes
