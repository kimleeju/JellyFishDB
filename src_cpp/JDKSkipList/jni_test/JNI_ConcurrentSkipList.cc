#include<jni.h>
#include<stdio.h>
#include<cstdio>
#include<pthread.h>
#include<iostream>


static JavaVM *vm;
static JNIEnv *env;
static JavaVMInitArgs vm_args;
static jclass jcls;

static jmethodID get_mid;
static jmethodID put_mid;	

void* run_jni_test(void* data){
	JNIEnv *t_env;
	jmethodID mid;	

	int get_env =vm->GetEnv((void **)&t_env, JNI_VERSION_1_2);
	if(get_env == JNI_EDETACHED){
		std::cout << "GetEnv: not attached" << std::endl;
        if (vm->AttachCurrentThread((void **) &t_env, NULL) != 0) {
            std::cout << "Failed to attach" << std::endl;
        }
	}
	
	jstring jstr = t_env->NewStringUTF("eunji");	
	mid = t_env->GetStaticMethodID(jcls, "Get", "(Ljava/lang/String;)Ljava/lang/String;");

	if (!mid) {
		printf("Error: unable to find Get\n");
	}

	jobject jrslt = t_env->CallStaticObjectMethod(jcls, mid, jstr);
	if (!jrslt) {
		printf("Error: failed to get value for key eunji\n");
	}else{
		const char* str = t_env->GetStringUTFChars((jstring) jrslt, NULL);
		printf(" t2 : %s\n", str);
	}
	vm->DetachCurrentThread();
}

void* run_jni(void* data){

	JNIEnv *t_env;
	jmethodID mid;
	int get_env =vm->GetEnv((void **)&t_env, JNI_VERSION_1_2);
	if(get_env == JNI_EDETACHED){
		std::cout << "GetEnv: not attached" << std::endl;
        if (vm->AttachCurrentThread((void **) &t_env, NULL) != 0) {
            std::cout << "Failed to attach" << std::endl;
        }
	}
	
/*		

	*/
	// put <k, v>
	jobjectArray jarr = t_env->NewObjectArray(2, 
							t_env->FindClass("java/lang/String"),
							t_env->NewStringUTF("str"));

	t_env->SetObjectArrayElement(jarr, 0, t_env->NewStringUTF("eunji"));
	t_env->SetObjectArrayElement(jarr, 1, t_env->NewStringUTF("seoul"));

/*
	mid = t_env->GetStaticMethodID(jcls, "Put", "([Ljava/lang/String;)V");

	if (!mid) {
		printf("Error: unable to find Put\n");
	}
*/
	t_env->CallStaticVoidMethod(jcls,put_mid, jarr);

	// get <k> returns <v> 
	jstring jstr = t_env->NewStringUTF("eunji");	
/*
	mid = t_env->GetStaticMethodID(jcls, "Get", "(Ljava/lang/String;)Ljava/lang/String;");
	if (!mid) {
		printf("Error: unable to find Get\n");
	}
*/
	jobject jrslt = t_env->CallStaticObjectMethod(jcls, get_mid, jstr);
	if (!jrslt) {
		printf("Error: failed to get value for key eunji\n");
	}

	const char* str = t_env->GetStringUTFChars((jstring) jrslt, NULL);
	printf("t1 : %s\n", str);

	vm->DetachCurrentThread();
	
}


int main(int argc, char** argv)
{
	pthread_t p_thread[2];
    int thr_id;
    int status;
    int a = 1;
    int b = 2;

	vm_args.version = JNI_VERSION_1_2;
	vm_args.nOptions = 0;

	jmethodID mid;

	jint res = JNI_CreateJavaVM(&vm, (void **)&env, &vm_args);
	if (res < 0) {
		printf("Error: failed to create jvm\n");
	}

	//jclass jcls = env->FindClass("java/util/concurrent/ConcurrentSkipListMap");
	jcls = env->FindClass("MyConcurrentSkipListMap");


	if (!jcls) {
		printf("Error: unable to find class \n");
	}

	// create skip list 
	mid = env->GetStaticMethodID(jcls, "create_sl", "()V");
	if (!mid){
		printf("Error: unable to find create_sl\n");
	}
	env->CallStaticVoidMethod(jcls, mid);

	put_mid = env->GetStaticMethodID(jcls, "Put", "([Ljava/lang/String;)V");

	if (!mid) {
		printf("Error: unable to find Put\n");
	}

	get_mid = env->GetStaticMethodID(jcls, "Get", "(Ljava/lang/String;)Ljava/lang/String;");
	if (!mid) {
		printf("Error: unable to find Get\n");
	}


    // 쓰레드 생성 아규먼트로 1 을 넘긴다.  
    thr_id = pthread_create(&p_thread[0], NULL,run_jni , (void*)&a);
    if (thr_id < 0)
    {
        perror("thread create error : ");
    }

    pthread_join(p_thread[0], (void **)&status);
    // 쓰레드 생성 아규먼트로 2 를 넘긴다. 
    thr_id = pthread_create(&p_thread[1], NULL, run_jni_test, (void*)&a);
    if (thr_id < 0)
    {
        perror("thread create error : ");
    }

    // 쓰레드 종료를 기다린다. 
    pthread_join(p_thread[1], (void **)&status);

	vm->DestroyJavaVM();
    return 0;

#if 0	
	//jmethodID mid = env->GetMethodID(jcls, "toLowerCase", "()Ljava/lang/String;");

	
	//const char* str = env->GetStringUTFChars((jstring) result, NULL);
	//printf("%s\n", str);
	//env->ReleaseStringUTFChars(jstr, str);
#endif
}

