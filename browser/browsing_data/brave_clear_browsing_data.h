/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this file,
* You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_BROWSING_DATA_BRAVE_CLEAR_BROWSING_DATA_H_
#define BRAVE_BROWSER_BROWSING_DATA_BRAVE_CLEAR_BROWSING_DATA_H_

namespace content {
class BrowsingDataRemover;
}
class Profile;
class TestClearDataOnExit;

namespace content {

class BraveClearBrowsingData {
public:
  static void ClearOnExit(Profile* profile);

protected:
  friend class ::TestClearDataOnExit;
  
  struct OnExitTestingCallback {
    virtual bool BeforeClearOnExitRemoveData(
        content::BrowsingDataRemover* remover,
        int remove_mask,
        int origin_mask) = 0;
  };
  static void SetOnExitTestingCallback(OnExitTestingCallback* callback);

private:
  static OnExitTestingCallback* on_exit_testing_callback_;
};

} // namespace content

#endif //BRAVE_BROWSER_BROWSING_DATA_BRAVE_CLEAR_BROWSING_DATA_H_
