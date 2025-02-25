using Microsoft.AspNetCore.Mvc;
using ShopApp.Models;
using ShopApp.Services;

// <summary>
// Provides endpoints for interacting with products
// </summary>
[Route("api/[controller]/[action]")]
[ApiController]
public class ProductsController : ControllerBase
{
    private readonly IProductService _products;
    public ProductsController(IProductService products)
    {
        _products = products;
    }
    /// <summary>
    /// Creates a product with the specified description and price.
    /// </summary>
    /// <param name="description">The description</param>
    /// <param name="price">The price of the product</param>
    /// <returns>Returns the created product if successful. Otherwise, it returns <c>null</c></returns>
    [HttpPost]
    public Product? CreateProduct(string description, double price)
    {
        if (string.IsNullOrEmpty(description) || price < 0)
        {
            return null;
        }
        var product = new Product()
        {
            Id = Guid.NewGuid(),
            Description = description,
            Price = price
        };
        return _products.Add(product);
    }

    /// <summary>
    /// Get Information about product by id
    /// </summary>
    /// <param name="id">The ID of the product</param>
    /// <returns>Returns product if the product exists. Otherwise, it returns <c>null</c></returns>
    [HttpGet]
    public Product? SearchProduct(Guid id)
    {
        return _products.Search(id);
    }

    /// <summary>
    /// Remove product by id
    /// </summary>
    /// <param name="id">The ID of the product</param>
    /// <returns>Returns the removed product if it exists. Otherwise, it returns <c>null</c></returns>
    [HttpPost]
    public Product? RemoveProduct(Guid id)
    {
        return _products.Remove(id);
    }

    /// <summary>
    /// Edit product by id, description and price
    /// </summary>
    /// <param name="id">The ID of the product</param>
    /// <param name="description">The new description</param>
    /// <param name="price">The new price of the product</param>
    /// <returns>Returns the edited product if it exists. Otherwise, it returns <c>null</c></returns>
    [HttpPost]
    public Product? EditProduct(Guid id, string description, double price)
    {
        var product = _products.Search(id);
        if (product == null || string.IsNullOrEmpty(description) || price < 0)
        {
            return null;
        }
        product.Description = description;
        product.Price = price;
        return _products.Edit(product);
    }
}   