using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace ModeManager
{
    public static class XboxCommunication
    {
        /// <summary>
        /// Sends files from a PC to an Xbox 360 Development Kit
        /// </summary>
        /// <param name="source">The source directory on the local PC</param>
        /// <param name="dest">The destination direction on the Xbox 360 Development Kit</param>
        /// <param name="console">The name/IP of the console (Defaults to your default console)</param>
        /// <param name="recursive">Copies files recursively.</param>
        /// <param name="searchSubDirectories">Searches in subdirectories.</param>
        /// <param name="includeHiddenFiles">Includes hidden files.</param>
        /// <param name="doesntPromptReplacing">Doesn't prompt when attempting to overwrite an existing file.</param>
        /// <param name="onlyCopiesNewer">Copies files only if newer than existing files with the same name.</param>
        /// <param name="dontCopyEmptyFolders">Doesn't copy empty directories.</param>
        /// <param name="createsDestinationIfNonExistant">Creates destination directory if it doesn't exist.</param>
        /// <param name="forceOverrideReadOnly">Forces overwrite of read-only files.</param>
        /// <param name="onlyCopyToSecureXdk">Copies files only if target console has been secured.</param>
        public static void XboxCopy(string source, string dest, string console = "", bool recursive = true, 
            bool searchSubDirectories = true, bool includeHiddenFiles = true, 
            bool doesntPromptReplacing = true, bool onlyCopiesNewer = false, bool dontCopyEmptyFolders = true,
            bool createsDestinationIfNonExistant = true, bool forceOverrideReadOnly = true,
            bool onlyCopyToSecureXdk = false)
        {
            var outputString = "{0} \"{1}\" \"{2}\"";

            var argumentString = "";
            if (console != string.Empty)
                argumentString += string.Format("/x {0} ", console);

            if (recursive)
                argumentString += string.Format("/r ");

            if (searchSubDirectories)
                argumentString += string.Format("/s ");

            if (includeHiddenFiles)
                argumentString += string.Format("/h ");

            if (doesntPromptReplacing)
                argumentString += string.Format("/y ");

            if (onlyCopiesNewer)
                argumentString += string.Format("/d ");

            if (dontCopyEmptyFolders)
                argumentString += string.Format("/e ");

            if (createsDestinationIfNonExistant)
                argumentString += string.Format("/t ");

            if (forceOverrideReadOnly)
                argumentString += string.Format("/f ");

            if (onlyCopyToSecureXdk)
                argumentString += string.Format("/m ");

            outputString = string.Format(outputString, argumentString, source, dest);

            Process.Start(System.Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86) + @"\Microsoft Xbox 360 SDK\bin\win32\xbcp.exe", outputString);
        }
    }
}
