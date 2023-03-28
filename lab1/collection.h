#ifndef LLP_COLLECTION_H
#define LLP_COLLECTION_H

#include <iosfwd>
#include <iostream>
#include "scheme.h"

using namespace std;

class collection_meta_info {
public:
    long start_documents;
    long end_documents; // also start of tree model records
    long end_tree_model_recs;
    bool is_empty;
    long collection_id;
    long offset_from_beg;
    static inline long current_id;

    long end_of_last_property;
    long end_of_last_node;

    collection_meta_info() = default;

    collection_meta_info(long offsetFromBeg, long start_docs, long end_docs, long end_coll) :
            collection_id(current_id++),
            offset_from_beg(offsetFromBeg),
            start_documents(start_docs),
            end_documents(end_docs),
            end_tree_model_recs(end_coll),
            is_empty(true),
            end_of_last_property(start_docs),
            end_of_last_node(end_docs)
    {}

    friend ostream& operator<<(ostream& os, const collection_meta_info& meta_inf) {
        cout << "collection_id: " << meta_inf.collection_id << endl;
        cout << "offset_from_beg: " << meta_inf.offset_from_beg << endl;
        cout << "start_documents: " << meta_inf.start_documents << endl;
        cout << "end_documents: " << meta_inf.end_documents << endl;
        cout << "end_tree_model_res: " << meta_inf.end_tree_model_recs << endl;
        cout << "empty collection: " << meta_inf.is_empty << endl;
        return os;
    }
};

class collection {
public:
    //size_t cur_num_docs; // dynamically  changed -- пока опционально, не сохраняю
    collection_meta_info header;
    scheme sch;
    // name <--> offset_from_region_beg
    map<string, long> existed_docs;
    map<string, long> deleted_docs;

    explicit collection(const scheme& sch) : sch(sch), header({}){}

    collection(const collection_meta_info &header, const scheme &scheme) :
            header(header), sch(scheme) {}

    collection(): sch({}, ""), header({}) {}


public:
    friend ostream& write(ostream& out, collection& col) {
        out.write(reinterpret_cast<char*>(&col.header), sizeof(col.header));
        col.sch.write_scheme_info(out);
        return out;
    }

    friend istream& read(istream& in, collection& col) {
        in.read(reinterpret_cast<char*>(&col.header), sizeof(col.header));
        col.sch.read_scheme_info(in);
        return in;
    }

    void write_collection(ostream& out) {
        out.seekp(this->header.offset_from_beg, ios_base::beg);
        write(out, *this);
    }

    void read_collection(istream& in, long offset) {
        in.seekg(offset, ios::beg);
        cout << "offset before READ collection: " << offset << endl;
        read(in, *this);
    }

    friend ostream& operator<<(ostream& os, const collection& meta_inf) {
        cout << meta_inf.header << endl;
        cout << "--collection scheme--" << endl;
        cout << meta_inf.sch;
        return os;
    }

    void set_header(const collection_meta_info &header) {
        collection::header = header;
    }

    long insert_node(fstream& file, doc_tree_info& node) {
        long node_tree_start = header.end_documents;
        node.add_node(file, node_tree_start);
        existed_docs.emplace(node.node_name, node_tree_start);
        return (long) file.tellp();
    }

    void remove_node(fstream& file, string& node_name) {
        long offset = existed_docs.at(node_name);
        //read node если у ноды есть дети, то не удаляем
        //deallocate
        // remove from existed documents
        // add to deleted documents
    }

    void update_node(fstream& file, string& node_name) {
        //////aaaaaaaa
    }


};


#endif //LLP_COLLECTION_H
