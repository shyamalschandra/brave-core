/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/browser_content_setting_bubble_model_delegate.h"
#include "brave/browser/ui/brave_browser_content_setting_bubble_model_delegate.h"
#include "brave/browser/ui/brave_browser_command_controller.h"
#include "brave/components/toolbar/brave_toolbar_model_impl.h"

namespace {

bool GetClearBrowsingDataOnExitSettings(Profile* profile,
                                        int& remove_mask,
                                        int& origin_mask) {
  PrefService* prefs = profile_->GetPrefs();
  remove_mask = 0;
  origin_mask = 0;

  int site_data_mask = ChromeBrowsingDataRemoverDelegate::DATA_TYPE_SITE_DATA;
  // Don't try to clear LSO data if it's not supported.
  if (!prefs->GetBoolean(prefs::kClearPluginLSODataEnabled))
    site_data_mask &= ~ChromeBrowsingDataRemoverDelegate::DATA_TYPE_PLUGIN_DATA;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteBrowsingHistoryOnExit) &&
    (prefs->GetBoolean(prefs::kAllowDeletingBrowserHistory))
    remove_mask |= ChromeBrowsingDataRemoverDelegate::DATA_TYPE_HISTORY;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteDownloadHistoryOnExit) &&
    prefs->GetBoolean(prefs::kAllowDeletingBrowserHistory))
    remove_mask |= content::BrowsingDataRemover::DATA_TYPE_DOWNLOADS;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteCacheOnExit))
    remove_mask |= content::BrowsingDataRemover::DATA_TYPE_CACHE;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteCookiesOnExit)) {
    remove_mask |= site_data_mask;
    origin_mask |= content::BrowsingDataRemover::ORIGIN_TYPE_UNPROTECTED_WEB;
  }

  if (prefs->GetBoolean(browsing_data::prefs::kDeletePasswordsOnExit))
    remove_mask |= ChromeBrowsingDataRemoverDelegate::DATA_TYPE_PASSWORDS;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteFormDataOnExit))
    remove_mask |= ChromeBrowsingDataRemoverDelegate::DATA_TYPE_FORM_DATA;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteHostedAppsDataOnExit)) {
    remove_mask |= site_data_mask;
    origin_mask |= content::BrowsingDataRemover::ORIGIN_TYPE_PROTECTED_WEB;
  }

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteMediaLicensesOnExit))
    remove_mask |= content::BrowsingDataRemover::DATA_TYPE_MEDIA_LICENSES;

  if (prefs->GetBoolean(browsing_data::prefs::kDeleteSiteSettingsOnExit))
    remove_mask |=
      ChromeBrowsingDataRemoverDelegate::DATA_TYPE_CONTENT_SETTINGS;

  return (remove_mask != 0);
}

void BraveClearBrowingDataOnExit(Profile* profile) {
  DCHECK(profile);

  // Chromium takes care of deleting browsing data of private profiles.
  if (profile_->IsOffTheRecord())
    return;

  // Check if any settings to clear data on exit have been turned on.
  int remove_mask = 0;
  int origin_mask = 0;
  if (!GetClearBrowsingDataOnExitSettings(profile, remove_mask, origin_mask))
    return;

  // Check if this is the last host for this profile.
  if ((ProfileDestroyer::GetHostsForProfile(profile)).size() > 1)
    return;

  // Remove data.

  content::BrowsingDataRemover* remover =
    content::BrowserContext::GetBrowsingDataRemover(profile_);
  remover->Remove(base::Time(), base::Time::Max(), remove_mask, origin_mask);

  //content::BrowserContext::GetBrowsingDataRemover(profile)->Remove(
  //  base::Time(), base::Time::Max(),
  //  ChromeBrowsingDataRemoverDelegate::WIPE_PROFILE,
  //  ChromeBrowsingDataRemoverDelegate::ALL_ORIGIN_TYPES);

  //browsing_data_important_sites_util::Remove(
  //  remove_mask, origin_mask, time_period,
  //  content::BrowsingDataFilterBuilder::Create(
  //    content::BrowsingDataFilterBuilder::BLACKLIST),
  //  remover, std::move(callback));
}

} // namespace

#define ToolbarModelImpl BraveToolbarModelImpl
#define BrowserContentSettingBubbleModelDelegate BraveBrowserContentSettingBubbleModelDelegate
#define BrowserCommandController BraveBrowserCommandController
#include "../../../../../chrome/browser/ui/browser.cc"
#undef BrowserContentSettingBubbleModelDelegate
#undef BrowserCommandController
#undef ToolbarModelImpl
