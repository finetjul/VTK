/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestSmartPointer.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME Test of Observers.
// .SECTION Description
// Tests vtkObject::AddObserver templated API

#include "vtkCallbackCommand.h"
#include "vtkNew.h"
#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include <map>

class vtkHandler : public vtkObject
{
public:
  static std::map<int, int> EventCounts;
  static int VoidEventCounts;
public:
  static vtkHandler* New();
  vtkTypeMacro(vtkHandler, vtkObject);

  void VoidCallback() { this->VoidEventCounts++; }
  void CallbackWithArguments(vtkObject*, unsigned long event, void*)
  {
    this->EventCounts[event]++;
  }
};
vtkStandardNewMacro(vtkHandler);

int vtkHandler::VoidEventCounts = 0;
std::map<int, int> vtkHandler::EventCounts;

class OtherHandler
{
public:
  static std::map<int, int> EventCounts;
  static int VoidEventCounts;
public:
  void VoidCallback() { this->VoidEventCounts++; }
  void CallbackWithArguments(vtkObject*, unsigned long event, void*)
  {
    this->EventCounts[event]++;
  }
};

int OtherHandler::VoidEventCounts = 0;
std::map<int, int> OtherHandler::EventCounts;

bool TestAddRemoveAndEventCounts()
{

  unsigned long event0 = 0;
  unsigned long event1 = 0;
  unsigned long event2 = 0;

  vtkObject* volcano = vtkObject::New();

  // First the base test, with a vtkObject pointer
  vtkHandler* handler = vtkHandler::New();

  event0 = volcano->AddObserver(
    1000, handler, &vtkHandler::VoidCallback);
  event1 = volcano->AddObserver(
    1001, handler, &vtkHandler::CallbackWithArguments);
  event2 = volcano->AddObserver(
    1002, handler, &vtkHandler::CallbackWithArguments);

  volcano->InvokeEvent(1000);
  volcano->InvokeEvent(1001);
  volcano->InvokeEvent(1002);

  // let's see if removing an observer works
  volcano->RemoveObserver(event2);
  volcano->InvokeEvent(1000);
  volcano->InvokeEvent(1001);
  volcano->InvokeEvent(1002);

  // now delete the observer, we shouldn't have any dangling pointers.
  handler->Delete();

  volcano->InvokeEvent(1000);
  volcano->InvokeEvent(1001);
  volcano->InvokeEvent(1002);

  // remove an observer after the handler has been deleted, should work.
  volcano->RemoveObserver(event1);
  volcano->InvokeEvent(1000);
  volcano->InvokeEvent(1001);
  volcano->InvokeEvent(1002);

  // remove the final observer
  volcano->RemoveObserver(event0);

  if (vtkHandler::VoidEventCounts == 2 &&
    vtkHandler::EventCounts[1000] == 0 &&
    vtkHandler::EventCounts[1001] == 2 &&
    vtkHandler::EventCounts[1002] == 1)
  {
    cout << "All vtkObject callback counts as expected." << endl;
  }
  else
  {
    cerr << "Mismatched callback counts for VTK observer." << endl;
    volcano->Delete();
    return false;
  }

  // ---------------------------------
  // Test again, with smart pointers
  vtkHandler::VoidEventCounts = 0;

  // Make a scope for the smart pointer
  {
    vtkSmartPointer<vtkHandler> handler2 = vtkSmartPointer<vtkHandler>::New();

    event0 = volcano->AddObserver(
      1003, handler2, &vtkHandler::VoidCallback);
    event1 = volcano->AddObserver(
      1004, handler2, &vtkHandler::CallbackWithArguments);
    event2 = volcano->AddObserver(
      1005, handler2, &vtkHandler::CallbackWithArguments);

    volcano->InvokeEvent(1003);
    volcano->InvokeEvent(1004);
    volcano->InvokeEvent(1005);

    // let's see if removing an observer works
    volcano->RemoveObserver(event2);
    volcano->InvokeEvent(1003);
    volcano->InvokeEvent(1004);
    volcano->InvokeEvent(1005);

    // end the scope, which deletes the observer
  }

  // continue invoking, to make sure that
  // no events to to the deleted observer
  volcano->InvokeEvent(1003);
  volcano->InvokeEvent(1004);
  volcano->InvokeEvent(1005);

  // remove an observer after the handler2 has been deleted, should work.
  volcano->RemoveObserver(event1);
  volcano->InvokeEvent(1003);
  volcano->InvokeEvent(1004);
  volcano->InvokeEvent(1005);

  // remove the final observer
  volcano->RemoveObserver(event0);

  if (vtkHandler::VoidEventCounts == 2 &&
    vtkHandler::EventCounts[1003] == 0 &&
    vtkHandler::EventCounts[1004] == 2 &&
    vtkHandler::EventCounts[1005] == 1)
  {
    cout << "All smart pointer callback counts as expected." << endl;
  }
  else
  {
    cerr << "Mismatched callback counts for smart pointer observer." << endl;
    volcano->Delete();
    return false;
  }

  // ---------------------------------
  // Test yet again, this time with a non-VTK object
  // (this _can_ leave dangling pointers!!!)

  OtherHandler *handler3 = new OtherHandler();

  event0 = volcano->AddObserver(
    1006, handler3, &OtherHandler::VoidCallback);
  event1 = volcano->AddObserver(
    1007, handler3, &OtherHandler::CallbackWithArguments);
  event2 = volcano->AddObserver(
    1008, handler3, &OtherHandler::CallbackWithArguments);

  volcano->InvokeEvent(1006);
  volcano->InvokeEvent(1007);
  volcano->InvokeEvent(1008);

  // let's see if removing an observer works
  volcano->RemoveObserver(event2);
  volcano->InvokeEvent(1006);
  volcano->InvokeEvent(1007);
  volcano->InvokeEvent(1008);

  // if we delete this non-vtkObject observer, we will
  // have dangling pointers and will see a crash...
  // so let's not do that until the events are removed

  volcano->RemoveObserver(event0);
  volcano->RemoveObserver(event1);
  delete handler3;

  // delete the observed object
  volcano->Delete();

  if (OtherHandler::VoidEventCounts == 2 &&
    OtherHandler::EventCounts[1006] == 0 &&
    OtherHandler::EventCounts[1007] == 2 &&
    OtherHandler::EventCounts[1008] == 1)
  {
    cout << "All non-VTK observer callback counts as expected." << endl;
    return true;
  }

  cerr << "Mismatched callback counts for non-VTK observer." << endl;
  return false;
}

/**
 * @brief Remove callback command @a clientData observation from invoker and
 * invoke event 1002 if @a event is not 1002.
 * @param clientData If a callbackCommand is given, its observation will be removed from the invoker.
 */
void removeObserverAndOrInvokeEvent(vtkObject* invoker, unsigned long event, void* clientData, void* vtkNotUsed(eventData))
{
  vtkCallbackCommand* callbackToRemove = reinterpret_cast<vtkCallbackCommand*>(clientData);
  if (callbackToRemove)
  {
    invoker->RemoveObserver(callbackToRemove);
  }

  if (event != 1002)
  {
    invoker->InvokeEvent(1002);
  }
}

/**
 * @brief Consider clientData as a bool* and set its value to true.
 */
void setClientDataToTrue(vtkObject* vtkNotUsed(invoker), unsigned long vtkNotUsed(event), void* clientData, void* vtkNotUsed(eventData))
{
  bool* clientDataBool = reinterpret_cast<bool*>(clientData);
  *clientDataBool = true;
}

/**
 * @brief This test makes sure that a removed observer is not processed
 * by the InvokeEvent loop.
 * @details The test should not crash.
 * @param removeObserverIn1000Or1002 true to remove observer in event 1000,
 * false to remove observer in event 1002
 * @sa removeObserverAndOrInvokeEvent() setClientDataToTrue()
 */
bool TestModifyList(bool removeObserverIn1000)
{
  vtkNew<vtkObject> invoker;

  vtkNew<vtkCallbackCommand> removeObserverAndInvokeCallback;
  removeObserverAndInvokeCallback->SetCallback(&removeObserverAndOrInvokeEvent);

  double priority = 2.0; // make sure it is the first to be processed
  invoker->AddObserver(1000, removeObserverAndInvokeCallback.Get(), priority);

  bool observerCalled = false; // must stay false
  vtkNew<vtkCallbackCommand> observerToBeRemoved;
  observerToBeRemoved->SetCallback(&setClientDataToTrue);
  observerToBeRemoved->SetClientData(&observerCalled);
  priority = 1.0; // make sure it is the second to be processed
  invoker->AddObserver(1000, observerToBeRemoved.Get(), priority);

  vtkNew<vtkCallbackCommand> removeObserverCallback;
  removeObserverCallback->SetCallback(&removeObserverAndOrInvokeEvent);
  priority = 0.0; // make sure it is the third to be processed
  invoker->AddObserver(1002, removeObserverCallback.Get(), priority);

  // observerToBeRemoved must be unobserved before being executed.
  if (removeObserverIn1000)
  {
    removeObserverAndInvokeCallback->SetClientData(observerToBeRemoved.Get());
  }
  else
  {
    removeObserverCallback->SetClientData(observerToBeRemoved.Get());
  }

  invoker->InvokeEvent(1000, NULL);

  return !observerCalled;
}


/**
 * @brief Runs all the tests of observers
 */
int TestObservers(int vtkNotUsed(argc), char* vtkNotUsed(argv)[])
{
  bool res = TestAddRemoveAndEventCounts();
  res = res && TestModifyList(false);
  res = res && TestModifyList(true);

  return res ? EXIT_SUCCESS : EXIT_FAILURE;
}
