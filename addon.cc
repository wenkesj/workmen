#include <nan.h>
#include <v8.h>
#include <uv.h>

using Nan::Persistent;
using v8::Function;
using v8::FunctionTemplate;
using v8::String;
using v8::Local;
using v8::Object;
using v8::Handle;
using Nan::HandleScope;
using Nan::ObjectWrap;

static Persistent<FunctionTemplate> constructor;
static Persistent<String> callback_symbol;

class Worker : public ObjectWrap {
  public:
    uv_work_t request;
    uv_async_t async;
    Persistent<Function> callback;
    bool working;

    Worker() { }
    ~Worker() { }

    static void AsyncProgress(uv_async_t* handle) {
      HandleScope scope;
      Worker* worker = static_cast<Worker*>(handle->data);
      Nan::MakeCallback(Nan::New<Object>(worker->persistent()), Nan::New<Function>(worker->callback), 0, {});
    }

    static void AsyncWork(uv_work_t* req) {
      Worker* worker = static_cast<Worker*>(req->data);
      do {
        uv_async_send(&worker->async);
      } while(worker->working);
    }

    static void AsyncAfter(uv_work_t* req, int status) {
      Worker* worker = static_cast<Worker*>(req->data);
      uv_close((uv_handle_t*) &worker->async, NULL);
    }

    static NAN_METHOD(New) {
      HandleScope scope;
      Worker* worker = new Worker();
      worker->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    }

    static NAN_METHOD(Open) {
      HandleScope scope;
      Worker *worker = ObjectWrap::Unwrap<Worker>(info.This());
      if (!info[0]->IsFunction()) {
        return Nan::ThrowTypeError("Expecting a function argument.");
      }

      uv_async_t async;

      worker->callback.Reset(info[0].As<Function>());
      worker->working = true;
      worker->async = async;
      worker->request.data = worker;
      worker->async.data = worker;

      uv_async_init(uv_default_loop(), &worker->async, AsyncProgress);
      uv_queue_work(uv_default_loop(), &worker->request, AsyncWork, AsyncAfter);
    }

    static NAN_METHOD(Close) {
      HandleScope scope;
      Worker *worker = ObjectWrap::Unwrap<Worker>(info.This());
      worker->working = false;
    }

    static void Initialize(Handle<Object> target) {
      HandleScope scope;
      Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
      constructor.Reset(tpl);
      tpl->InstanceTemplate()->SetInternalFieldCount(1);
      tpl->SetClassName(Nan::New<String>("Worker").ToLocalChecked());
      Nan::SetPrototypeMethod(tpl, "open", Open);
      Nan::SetPrototypeMethod(tpl, "close", Close);
      target->Set(Nan::New<String>("Worker").ToLocalChecked(), tpl->GetFunction());
    }
};

extern "C" {
  void init(Handle<Object> target) {
    HandleScope scope;
    Worker::Initialize(target);
  }

  NODE_MODULE(worker, init);
}
