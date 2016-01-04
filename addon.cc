#include <node.h>
#include <nan.h>
#include <v8.h>
#include <uv.h>

using Nan::Persistent;
using v8::Function;
using v8::FunctionTemplate;
using v8::internal::Arguments;
using v8::Exception;
using v8::String;
using v8::Local;
using Nan::Undefined;
using v8::Value;
using Nan::Null;
using v8::Object;
using v8::Handle;
using Nan::HandleScope;
using v8::Number;
using Nan::TryCatch;
using v8::Context;
using v8::FunctionTemplate;
using v8::Isolate;

static Nan::Persistent<FunctionTemplate> constructor;
static Nan::Persistent<String> callback_symbol;

class Baton : public Nan::ObjectWrap {
  public:
    uv_work_t request;
    uv_async_t async;
    Persistent<Function> callback;
    bool working;

    Baton() { }
    ~Baton() { }

    static void AsyncProgress(uv_async_t* handle) {
      HandleScope scope;
      Baton* baton = static_cast<Baton*>(handle->data);
      Nan::MakeCallback(Nan::New<Object>(baton->persistent()), Nan::New<Function>(baton->callback), 0, {});
    }

    static void AsyncWork(uv_work_t* req) {
      Baton* baton = static_cast<Baton*>(req->data);
      do {
        uv_async_send(&baton->async);
      } while(baton->working);
    }

    static void AsyncAfter(uv_work_t* req, int status) {
      Baton* baton = static_cast<Baton*>(req->data);
      uv_close((uv_handle_t*) &baton->async, NULL);
    }

    static NAN_METHOD(New) {
      Nan::HandleScope scope;
      Baton* baton = new Baton();
      baton->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    }

    static NAN_METHOD(Open) {
      Nan::HandleScope scope;
      Baton *baton = Nan::ObjectWrap::Unwrap<Baton>(info.This());
      if (!info[0]->IsFunction()) {
        return Nan::ThrowTypeError("Expecting a function argument.");
      }

      uv_async_t async;

      baton->callback.Reset(info[0].As<Function>());
      baton->working = true;
      baton->async = async;
      baton->request.data = baton;
      baton->async.data = baton;

      uv_async_init(uv_default_loop(), &baton->async, AsyncProgress);
      uv_queue_work(uv_default_loop(), &baton->request, AsyncWork, AsyncAfter);
    }

    static NAN_METHOD(Defer) {
      Nan::HandleScope scope;
      Baton *baton = Nan::ObjectWrap::Unwrap<Baton>(info.This());
      baton->working = false;
    }

    static void Initialize(Handle<Object> target) {
      Nan::HandleScope scope;
      Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
      constructor.Reset(tpl);
      tpl->InstanceTemplate()->SetInternalFieldCount(1);
      tpl->SetClassName(Nan::New<String>("Baton").ToLocalChecked());
      Nan::SetPrototypeMethod(tpl, "open", Open);
      Nan::SetPrototypeMethod(tpl, "defer", Defer);
      target->Set(Nan::New<String>("Baton").ToLocalChecked(), tpl->GetFunction());
    }
};

extern "C" {
  void init(Handle<Object> target) {
    Nan::HandleScope scope;
    Baton::Initialize(target);
  }

  NODE_MODULE(baton, init);
}
