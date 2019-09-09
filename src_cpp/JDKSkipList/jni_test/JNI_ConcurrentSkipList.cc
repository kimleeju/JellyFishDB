#include<jni.h>
#include<stdio.h>

int main(int argc, char** argv)
{
	JavaVM *vm;
	JNIEnv *env;
	JavaVMInitArgs vm_args;

	vm_args.version = JNI_VERSION_1_2;
	vm_args.nOptions = 0;

	// Construct a VM
	jint res = JNI_CreateJavaVM(&vm, (void **)&env, &vm_args);
	if (res < 0) {
		printf("Error: failed to create jvm\n");
		return 0;
	}

	//jclass jcls = env->FindClass("java/util/concurrent/ConcurrentSkipListMap");
	jclass jcls = env->FindClass("MyConcurrentSkipListMap");


	if (!jcls) {
		printf("Error: unable to find class \n");
		return 0;
	}

	// create skip list 
	jmethodID mid = env->GetStaticMethodID(jcls, "create_sl", "()V");
	if (!mid){
		printf("Error: unable to find create_sl\n");
		return 0;
	}
	env->CallStaticVoidMethod(jcls, mid);

	// put <k, v>
	jobjectArray jarr = env->NewObjectArray(2, 
							env->FindClass("java/lang/String"),
							env->NewStringUTF("str"));

	env->SetObjectArrayElement(jarr, 0, env->NewStringUTF("eunji"));
	env->SetObjectArrayElement(jarr, 1, env->NewStringUTF("seoul"));

	mid = env->GetStaticMethodID(jcls, "Put", "([Ljava/lang/String;)V");

	if (!mid) {
		printf("Error: unable to find Put\n");
		return 0;
	}

	env->CallStaticVoidMethod(jcls, mid, jarr);

	// get <k> returns <v> 
	jstring jstr = env->NewStringUTF("eunji");	
	mid = env->GetStaticMethodID(jcls, "Get", "(Ljava/lang/String;)Ljava/lang/String;");

	if (!mid) {
		printf("Error: unable to find Get\n");
		return 0;
	}

	jobject jrslt = env->CallStaticObjectMethod(jcls, mid, jstr);
	if (!jrslt) {
		printf("Error: failed to get value for key eunji\n");
		return 0;
	}

	const char* str = env->GetStringUTFChars((jstring) jrslt, NULL);
	printf("%s\n", str);

	//env->ReleaseStringUTFChars(jstr, str);

//	jclass jcls_r = env->FindClass("ConcurrentSkipListMap");


#if 0	
	//jmethodID mid = env->GetMethodID(jcls, "toLowerCase", "()Ljava/lang/String;");

	
	//const char* str = env->GetStringUTFChars((jstring) result, NULL);
	//printf("%s\n", str);
	//env->ReleaseStringUTFChars(jstr, str);
#endif
	vm->DestroyJavaVM();
}

