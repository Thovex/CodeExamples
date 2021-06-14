
#pragma once

#define LOG_CUR_CLASS_FUNC (FString(__FUNCTION__))
#define LOG_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )
#define LOG_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))
#define LOG_CUR_LINE  (FString::FromInt(__LINE__))
#define LOG_CUR_CLASS_LINE (LOG_CUR_CLASS + "(" + LOG_CUR_LINE + ")")
#define LOG_CUR_FUNCSIG (FString(__FUNCSIG__))

#define LOG_PRINT_CURRENT_CLASS_LINE if(GEngine) GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, *( "ERROR " + LOG_CUR_CLASS_LINE ) )
#define LOG_SCREENMSG(Param1) if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(LOG_CUR_CLASS_LINE + ": " + Param1)) )

#define RETURN_IF_NULLPTR(Object) if (!Object) { LOG_PRINT_CURRENT_CLASS_LINE; return; }
#define RETURN_FALSE_IF_NULLPTR(Object) if (!Object) { LOG_PRINT_CURRENT_CLASS_LINE; return false; }
#define RETURN_TRUE_IF_NULLPTR(Object) if (!Object) { LOG_PRINT_CURRENT_CLASS_LINE; return true; }

#define GET_BLUEPRINT(Class, VariableName, Directory) static ConstructorHelpers::FClassFinder<Class> VariableName( TEXT(Directory) )

#define MAX_FLOAT 340282346638528870000000000000000000000.000000F
#define MIN_FLOAT -MAX_FLOAT

#define PRINT(string) if (GEngine) GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, string)
#define PRINTINT(integer) PRINT(FString::FromInt(integer));
#define PRINTG(string) if (GEngine) GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Green, string)
#define PRINTT(string, time) if (GEngine) GEngine->AddOnScreenDebugMessage( -1, time, FColor::Red, string)

#define INI_COMP(ComponentPtr, Class, ComponentName) ComponentPtr = ObjectInitializer.CreateDefaultSubobject<Class>(this, TEXT(ComponentName));
#define INI_COMP_ATTACH(ComponentPtr, RootPtr, Class, ComponentName) INI_COMP(ComponentPtr, Class, ComponentName) ComponentPtr->SetupAttachment(RootPtr);