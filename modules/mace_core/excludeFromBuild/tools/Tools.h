#pragma once

// makes it illegal to copy a derived class
// https://github.com/heisters/libnodes
struct Noncopyable
{
 protected:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable (const Noncopyable&) = delete;
    Noncopyable& operator= (const Noncopyable&) = delete;
};

// provides derived classes with automatically assigned,
// globally unique numeric identifiers
// https://github.com/heisters/libnodes
class HasId
{
 public:
    HasId() :
        mId (++sId) {}

    ItemID id() const { return mId; }
    void setID (ItemID itemID) { mId = itemID; }

    void staticReset (int id = 0) { sId = id; }

 protected:
    static ItemID sId;
    ItemID mId;
};

// from the Code Blacksmith
// https://www.youtube.com/watch?v=GV0JMHOpoEw
class ScopedStopWatch
{
 public:
    using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                     std::chrono::high_resolution_clock,
                                     std::chrono::steady_clock>;
    ScopedStopWatch (const char function[] = "unknown function") :
        func (function)
    {
    }
    ~ScopedStopWatch()
    {
        LOG (DBUG) << "\n"
                   << func << " took " << std::chrono::duration_cast<std::chrono::milliseconds> (Clock::now() - start).count() << " milliseconds";
    }

 private:
    const char* func = nullptr;
    Clock::time_point start = Clock::now();
};

// store and retrieve any type from a map
template <class PROPERTY>
class AnyValue
{
    using ValueMap = std::unordered_map<int, std::any>;

 public:
    AnyValue() = default;
    ~AnyValue() = default;

    void addDefault (const PROPERTY& key, const std::any& value) { map_.insert (std::make_pair (key, value)); }
    void setValue (const PROPERTY& key, const std::any& value)
    {
        auto it = map_.find (key);
        if (it == map_.end())
            map_.insert (std::make_pair (key, value));
        else
            it->second = value;
    }

    template <typename T>
    T& getRef (const PROPERTY& key) { return std::any_cast<T> (getValue (key)); }

    template <typename T>
    T getVal (const PROPERTY& key) { return std::any_cast<T> (getValue (key)); }

    template <typename T>
    T* getPtr (const PROPERTY& key) { return std::any_cast<T> (&getValue (key)); }

 private:
    ValueMap map_;
    std::any empty_;

    std::any& getValue (const PROPERTY& key)
    {
        auto it = map_.find (key);
        if (it != map_.end())
            return it->second;

        return empty_;
    }

}; // end class AnyValue

struct FileServices
{
    static void copyFiles (const juce::String& searchFolder, const juce::String& destFolder, const juce::String& wildcards, bool recursive = true)
    {
        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (juce::File (searchFolder), recursive, wildcards))
        {
            juce::String dest = destFolder + "/" + entry.getFile().getFileName();
            entry.getFile().copyFileTo (dest);
        }
    }

    static void moveFiles (const juce::String& searchFolder, const juce::String& destFolder, const juce::String& wildcards)
    {
        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (juce::File (searchFolder), true, wildcards))
        {
            juce::String dest = destFolder + "/" + entry.getFile().getFileName();
            entry.getFile().moveFileTo (dest);
        }
    }

    static void getFiles (const juce::String& searchFolder, juce::StringArray& filesToGet, const juce::String& wildcards, bool recursive = true)
    {
        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (juce::File (searchFolder), recursive, wildcards))
        {
            filesToGet.add (entry.getFile().getFullPathName());
        }
    }

    static void getFilesAndFolders (const juce::String& searchFolder, juce::StringArray& filesToGet, const juce::String& wildcards, bool recursive = true)
    {
        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (juce::File (searchFolder), recursive, wildcards))
        {
            filesToGet.add (entry.getFile().getFullPathName());
        }
    }

    static void getFolders (const juce::String& searchFolder, juce::StringArray& foldersToGet, bool recursive = true)
    {
        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (juce::File (searchFolder), recursive, "*", juce::File::findDirectories))
        {
            foldersToGet.add (entry.getFile().getFullPathName());
        }
    }

    static void copyFolder (const juce::String& folderToCopy, const juce::String& destFolder, const juce::String& wildcards)
    {
        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (juce::File (folderToCopy), true, wildcards))
        {
            juce::String dest = destFolder + "/" + entry.getFile().getFileName();
            entry.getFile().copyFileTo (dest);
        }
    }

    static void getTextFileLines (const juce::String& filePath, juce::StringArray& lines)
    {
        juce::File file (filePath);
        if (!file.exists())
            return;
        else
            file.readLines (lines);
    }

    static juce::String findFilePath (const juce::String& searchFolder, const juce::String& fileName)
    {
        for (juce::DirectoryEntry entry : juce::RangedDirectoryIterator (juce::File (searchFolder), true))
        {
            if (entry.getFile().getFileName() == fileName)
                return entry.getFile().getFullPathName();
        }

        return juce::String();
    }
};