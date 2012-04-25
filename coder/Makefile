app_name = encoder
code_dir = code
test_dir = test
in_ext = .in
enc_ext = .enc
dec_ext = .dec
report_fname = report
source_files = $(wildcard $(code_dir)/*.c)
test_files = $(wildcard $(test_dir)/*$(in_ext))
obj_files = $(source_files:.c=.o)

$(app_name):$(obj_files)
	gcc $(obj_files) -o $@

.PHONY: test
test:$(app_name) 
	$(foreach var, $(test_files),\
		./$(app_name) 3 3 -e < $(var) > $(patsubst %$(in_ext), %$(enc_ext), $(var));\
		./$(app_name) 3 3 -d < $(patsubst %$(in_ext), %$(enc_ext), $(var)) > $(patsubst %$(in_ext), %$(dec_ext), $(var));\
		diff -q $(patsubst %$(in_ext), %$(dec_ext), $(var)) $(var) && echo "PASSED";\
		if [ $$? -eq 1 ] ; then echo "FAILED" ; fi;\
	)
.PHONY: clean
clean:
	rm -rf $(app_name) $(test_dir)/*$(enc_ext) $(test_dir)/*$(dec_ext)  $(code_dir)/*.o $(code_dir)/*~
